// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
// CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE

#include <ar.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <errno.h>


struct meta{
    char name[16]; // room for null
    int mode;
    int size;
    time_t mtime; // a time_t is a long
};

// stats the file name and fills in ar_hdr structure
int fill_ar_hdr(char* filename, struct ar_hdr* hdr){
    int fd = open(filename, O_RDONLY); // open file for reading
    if (fd == -1) {
        perror("Error opening regular file for archive header.");
        close(fd);
        return 1;
    }
    
    //Use stat to get file metadata
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) { // fstat will write info from the file descriptor into file_stat
        perror("Error getting file metadata for archive header.");
        close(fd);
        return 1;
    }

    //Copy over data from file_stat to ar_hdr
    strcpy(hdr->ar_name, filename);
    sprintf(hdr->ar_date, "%ld", file_stat.st_mtime); // converted to decimal string
    sprintf(hdr->ar_uid, "%d", file_stat.st_uid); //converted to decimal string
    sprintf(hdr->ar_gid, "%d", file_stat.st_gid); //converted to decimal string
    sprintf(hdr->ar_mode, "%o", file_stat.st_mode); //converted to octal string
    sprintf(hdr->ar_size, "%lld", file_stat.st_size); //converted to decimal string
    strcpy(hdr->ar_fmag, ARFMAG); // file magic string, defined in ar.h

    close(fd);
    return 0;



}
// uses ar_hdr to fill in meta structure
int fill_meta(struct ar_hdr hdr, struct meta* meta){
    strcpy(meta->name, hdr.ar_name);
    meta->mode = strtol(hdr.ar_mode, NULL, 8); //octal so base 8
    meta->size = strtol(hdr.ar_size, NULL, 10);
    meta->mtime = (time_t) strtol(hdr.ar_date, NULL, 10);
    return 0;

}
// lists all file names in an archive file
int list_files(char* archive_file){
    int archive_fd;
    if ( (archive_fd = open(archive_file, O_RDONLY)) == -1){
        perror("Error opening archive file for listing.");
        return 1;
    }

    struct ar_hdr hdr;
    struct meta meta;
    off_t offset = lseek(archive_fd, SARMAG, SEEK_SET); // skip the magic string at beginning of archive file
    while (read(archive_fd, &hdr, sizeof(hdr)) > 0) {
        fill_meta(hdr, &meta);
        printf("%s\n", meta.name);
        offset += sizeof(hdr) + meta.size; // move offset to the beginning of the next header
        lseek(archive_fd, offset, SEEK_SET); // move file pointer to the beginning of the next header
    }
    close(archive_fd);
    return 0;
}
// quickly appends specified files to the archive modified more than N days ago
int append_files(char* archive_file, char* files[], int num_files, int N){
    //open archive file or create a new one if it doesn't exist
    int archive_fd, file_fd;
    if (( archive_fd = open(archive_file, O_WRONLY | O_CREAT, 0666)) == -1){
        perror("Error opening or creating archive file.");
        return 1;
    }

    struct ar_hdr hdr;
    struct stat stat_buffer;



    for (int i = 0; i < num_files; i++) {
        if (stat(files[i], &stat_buffer) == -1) {
            perror("Error getting file metadata for appending.");
            return 1;
        }
        if ( (time(NULL) - stat_buffer.st_mtime) > (N * 60 * 60 * 24) ){
            //write header
            fill_ar_hdr(files[i], &hdr);
            write(archive_fd, &hdr, sizeof(hdr));
            char contents_buffer[stat_buffer.st_size];


            //write file contents
           if( ( file_fd = open(files[i], O_RDONLY)) == -1){
                perror("Error opening file for appending.");
                return 1;
            }
            read(file_fd, &contents_buffer, stat_buffer.st_size);
            write(archive_fd, &contents_buffer, stat_buffer.st_size);
            close(file_fd);
        }  

    }
    close(archive_fd);
    return 0;
}
// extracts files and optionally restores permissions with -o
int extract_files(char* archive_file, char* files[], int num_files, int restore_permissions){
    int archive_fd, file_fd;
    if (( archive_fd = open(archive_file, O_RDONLY)) == -1){
        perror("Error opening archive file for extraction.");
        return 1;
    }

    struct ar_hdr hdr;
    struct stat stat_buffer;
    off_t offset = lseek(archive_fd, SARMAG, SEEK_SET); // skip the magic string at beginning of archive file
    int match = 0;
    for (int i = 0; i < num_files; i++){
        while (read(archive_fd, &hdr, sizeof(hdr)) > 0) {
            if (strcmp(hdr.ar_name, files[i]) == 0) {
                match = 1;
                //write file contents
                char contents_buffer[strtol(hdr.ar_size, NULL, 10)];
                read(archive_fd, &contents_buffer, strtol(hdr.ar_size, NULL, 10));
                if ( ( file_fd = open(files[i], O_WRONLY | O_CREAT, 0666)) == -1){
                    perror("Error opening or creating file for extraction.");
                    return 1;
                }
                write(file_fd, &contents_buffer, strtol(hdr.ar_size, NULL, 10));
                close(file_fd);

                //optionally restore permissions and timestamps
                if (restore_permissions){
                    if (chmod(files[i], strtol(hdr.ar_mode, NULL, 8)) == -1){
                        perror("Error restoring permissions.");
                        return 1;
                    }
                    struct utimbuf time_buffer;
                    time_buffer.actime = (time_t) strtol(hdr.ar_date, NULL, 10);
                    time_buffer.modtime = (time_t) strtol(hdr.ar_date, NULL, 10);
                    if (utime(files[i], &time_buffer) == -1){
                        perror("Error restoring timestamps.");
                        return 1;
                    }
                }
                // skip to the next header
                offset += sizeof(hdr) + strtol(hdr.ar_size, NULL, 10); // move offset to the beginning of the next header
                lseek(archive_fd, offset, SEEK_SET); // move file pointer to the beginning of the next header
            }
        }

        if (!match){
            printf("File %s not found in archive.\n", files[i]);
        }
        //next file
    }
    close(archive_fd);
    return 0;     
 }

int main(int argc, char* argv[]) {
    

    if (argc < 3) {
        fprintf(stderr, "Usage: myar x[o]tA archive-file [file1 ...]\n");
        return 1;
    }

    int option;
    int option_o;
    char* arfile = argv[2]; // name of the archive file to be used

    if ((option = getopt(argc, argv,"[xotA:]" )) != -1) {
        switch(option)
        {
        case 't': // list all files in the archive
            list_files(arfile);
            break;
        case 'x':// Extract files and optionally restore permissions with -o
            option_o = getopt(argc, argv, "o");
            extract_files(arfile, &argv[3], argc - 3, option_o == 'o');
            break;
        case 'A': //quickly append specified files to the archive modified more than N days ago
            append_files(arfile, &argv[3], argc - 3, 3);
            break;
        case '?': // An unknown option was given
            fprintf(stderr, "Invalid option: %c\n", option);
            return 1;
        }
    }

    return 0;
}