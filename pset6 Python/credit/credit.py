from cs50 import get_string

cardNumber = get_string("Please input your credit card number: ")

# get numbers from 0 to 2
first2Digits = int(cardNumber[:2])
# get length of number
length = len(cardNumber)

# Luhn's algorith
sum = 0
for i in range(1, length + 1):
    # take one digit from the number starting from the end
    digit = int(cardNumber[-i])
    # for even numbers
    if i % 2 == 0:
        # if number has 2 digits
        if digit * 2 > 9:
            # -9 is the same as adding both digits of the number
            sum += digit * 2 - 9
        else:
            sum += digit * 2
    # for odd numbers
    else:
        sum += digit

# check if digit after Luhn's algorithm ends with 0 if y = true, no = false
if sum % 10 == 0:
    flag = True
else:
    flag = False

if length == 15 and (first2Digits == 34 or first2Digits == 37) and flag:
    print("AMEX")
elif length == 16 and 50 < first2Digits < 56 and flag:
    print("MASTERCARD")
elif int(cardNumber[:1]) == 4 and (length == 13 or length == 16) and flag:
    print("VISA")
else:
    print("INVALID")