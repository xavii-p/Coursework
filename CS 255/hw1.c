#include<stdio.h>
#include<stdlib.h>


void rem(const char *s)
{
    printf("%s\n", s); // prints the original word

    for (int i = 0; s[i]; i++) // loops through each letter in the string and checks for vowels
    {
        if (s[i] != 'a' && s[i] != 'e' && s[i] != 'i' && s[i] !=  'o' && s[i] != 'u' && s[i] != 'A' && s[i] != 'E' && s[i] !=  'I' && s[i] != 'O' && s[i] != 'U')
        {
            printf("%c", s[i]); // if the letter is not a vowel it is printed on the same line as previous letters
        }
    }
    printf("\n"); // new line for next word/input
}

void perfect(long num)
{
    if (num != 0) // 0 cannot be a perfect number
    {
    long currentinteger = 1; // index of the integer we are checking to see if it is a factor of the number that was given
    long sum = 0; // sum for adding the factors of num
    while (currentinteger < num) // loop will run for all factors less than num
    {
        if (num % currentinteger == 0) // checks to see if currentinteger is a factor of num
        {
            sum += currentinteger; // if it is a factor,  it adds this integer to the sum
        }
        currentinteger++; // move to the next integer and check to see if it is a factor
    }
    if (sum == num) // the sum of the factors must equal the given number to be a perfect number
    {
        printf("%ld is perfect\n", num); // num was given as a long hence %ld

    } else
        printf("%ld is not perfect\n", num);
    } else 
        printf("0 is not perfect\n"); // print statement if 0 is given as input
}

void nearest(void)
{
    int running = 1;
    int nearestprimebelow = 1;
    int nearestprimeabove = 1;
    while (running == 1) // infinite loop to be to ask for more input
    {
        printf("Enter a positive integer: "); // prompt user for input
        char array[2]; // 
        char input = getchar(); // store input from user
        array[0] = input; // place this input into char array to be able to convert it from string to int
        int val = atoi(array); // convert from string to int to preserve actual value of the input and not get the ASCII value
        for (int i = 2; i * i <= val; i++)
        {
            if (val % i == 0)
            {
                nearestprimebelow = 2;
            } 
            printf("The number %d is prime.", val);
            break;
         }
            

        for (int i = val + 1; i < val * val; i++)
        {
            if (val % i/2 == 0)
            {
                nearestprimeabove = 0;

            } else 
                nearestprimeabove = i;
        }
        if ((nearestprimeabove - val) > (val - nearestprimebelow))
        {
            printf("The nearest prime number to %d is %d\n", val, nearestprimebelow);
        } else if ((nearestprimeabove - val) < (val - nearestprimebelow)){
            printf("The nearest prime number to %d is %d\n", val, nearestprimeabove);
        } else 
            printf("The nearest prime number to %d is %d\n", val, nearestprimebelow);
            
    }

}