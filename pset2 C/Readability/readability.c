#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");

    // init variables
    float letters = 0;
    //1 space = 2 words etc
    float words = 1;
    float sentences = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        //checks for an alphabetic character
        if (isalnum(text[i]))
        {
            letters++;
        }
        //checks for white-space characters
        //works if sentence ends and starts without white-spaces and there are no double white-spaces
        else if (isspace(text[i]))
        {
            words++;
        }
        //punctuation = end of sentence
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    //Coleman-Liau index
    float grade = 0.0588 * (100 * letters / words) - 0.296 * (100 * sentences / words) - 15.8;

    //print messages
    if (grade < 16 && grade >= 1)
    {
        printf("Grade %i\n", (int) round(grade));
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }
}