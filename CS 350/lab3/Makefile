TARGET=manage compute report
CC=gcc

.PHONY: all clean

all: $(TARGET)

manage: manage.o
	$(CC) -o $@ $^

compute: compute.o
	$(CC) -o $@ $^

report: report.o
	$(CC) -o $@ $^

# Object files depend on respective C files and header files
manage.o: manage.c defs.h
	$(CC) -c -o $@ $<

compute.o: compute.c defs.h
	$(CC) -c -o $@ $<

report.o: report.c defs.h
	$(CC) -c -o $@ $<

clean:
	$(RM) $(TARGET) *.o
