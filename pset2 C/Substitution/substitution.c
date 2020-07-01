#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

bool validate(string key, int keyLength); //declaration

//main function for substitution
int main(int argc, string argv[])
{
    //checks for correct number of arguments (1)
    if (argc == 2)
    {
        //variables for provided key
        int keyLength = strlen(argv[1]);
        string key = argv[1];
        if (validate(key, keyLength))
        {
            //variables for provided text
            string plainText = get_string("plaintext: ");
            int textLength = strlen(plainText);

            //length + place for null terminator to make it a string
            char cipherText[textLength + 1];

            //loops through letters of plainText
            for (int i = 0; i < textLength; i++)
            {
                //if uppercase
                if (isupper(plainText[i]) != 0)
                {
                    //loops as many times as there are keys
                    for (int j = 0; j < 26; j++)
                    {
                        //each loop goes one letter. Find number of letter belonging to plainText (0-a, 1-b, 2-c etc) = j
                        if (plainText[i] == 'A' + j)
                        {
                            //assign letter from key with index j to our cipherText array
                            cipherText[i] = toupper(key[j]);
                            //break out of the loop prematurely if we assigned letter
                            break;
                        }
                    }
                }
                //if lowercase do the same
                else if (islower(plainText[i]) != 0)
                {
                    for (int j = 0; j < 26; j++)
                    {
                        if (plainText[i] == 'a' + j)
                        {
                            cipherText[i] = tolower(key[j]);
                            break;
                        }
                    }

                }
                else
                {
                    //if not alphabetic add symbols to array as they are
                    cipherText[i] = plainText[i];
                }
            }
            // char type array has no \0, add to make string out of array
            cipherText[textLength] = '\0';
            //print cipherText
            printf("ciphertext: %s\n", cipherText);
            return 0;
        }
        else
        {
            //error message if validation not successful
            printf("Key must contain 26 different alphabetic characters\n");
            return 1;
        }
    }
    else
    {
        //error message if amount of arguments passed after command is not correct
        printf("Usage: %s KEY\n", argv[0]);
        return 1;
    }

}

//validate function, returns bool
bool validate(string key, int keyLength)
{
    //check length
    if (keyLength == 26)
    {
        //loop through key letters and check if alphabetic
        int matchingLetters = 0;
        for (int i = 0; i < keyLength; i++)
        {
            if (isalpha(key[i]))
            {
                //loop through key once more. Check for matching letters
                for (int j = 0; j < keyLength; j++)
                {
                    //if key of the first loop and of this loop match - there is matching letter
                    if (key[j] == key[i])
                    {
                        matchingLetters++;
                    }
                }
            }
        }
        //if matchingLetters > 26 - not all letters in the key are unique
        if (matchingLetters == 26)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}