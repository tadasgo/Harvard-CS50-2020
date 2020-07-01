#include <cs50.h>
#include <stdio.h>

int main(void) 
{
    int height; 
    do 
    {
        //get user input and set it to height
        height = get_int("Please input height of pyramid (1 to 8, inclusive): ");
    }
    while (height < 1 || height > 8);
    //rows
    for (int i = 0; i < height; i++) 
    {
        int spaces, hashes;
        //amount of spaces top to bottom = height - number of row
        for (spaces = (height - i - 1); spaces > 0; spaces--)
        {
            printf(" ");
        }
        //amount of hashes = amount of rows 
        for (hashes = 0; hashes <= i; hashes++)
        {
            printf("#");    
        }
        printf("  ");
        for (hashes = 0; hashes <= i; hashes++)
        {
            printf("#");    
        }
        printf("\n");
    }
}