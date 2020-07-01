#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask for card number and set it to variable card_n
    long long card_n;
    card_n = get_long("Please input your credit card number: ");
    //copy for calculations
    long long temp_n = card_n;
    //start at 2, because in while condition 100, not 0
    //count = amount of numbers
    //temp_n = last 2 numbers
    int count = 2;
    while (temp_n > 100LL)
    {
        temp_n = temp_n / 10LL;
        count ++;
    }
    //copy2 for calculations
    long long temp_n2 = card_n;
    int sum = 0;

    // Luhn's algorithm
    for (int i = 1; i <= count; i++)
    {
        //get last number
        int digit = temp_n2 % 10LL;
        //for even numbers
        if (i % 2 == 0)
        {
            //if number has 2 digits
            if (digit * 2 > 9)
            {
                //-9 is the same as adding both digits of the number
                sum += (digit * 2) - 9;
            }
            else
            {
                sum += digit * 2;
            }
        }
        //for odd numbers
        else
        {
            sum += digit;
        }
        //remove the digit we already used
        temp_n2 /= 10LL;
    }
    //check if digit after Luhn's algorithm ends with 0 if y = true, no = false
    bool flag = (sum % 10 == 0) ? true : false;

    if (count == 15 && (temp_n == 34 || temp_n == 37) && flag)
    {
        printf("AMEX\n");
    }
    else if (count == 16 && (temp_n > 50 && temp_n < 56) && flag)
    {
        printf("MASTERCARD\n");
    }
    else if ((count == 13 || count == 16) && (temp_n / 10 == 4) && flag)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    } 
}