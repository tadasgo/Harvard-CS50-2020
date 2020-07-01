from cs50 import get_string

text = get_string("Text: ")

# iterate each char in text, set to letter, check if letter is alphanumeric
letters = 0
for letter in text:
    if letter.isalnum():
        letters += 1

# count words
words = len(text.split())

# count .!? - one sign = one sentence
sentences = text.count('.') + text.count('!') + text.count('?')

# Coleman-Liau index
grade = 0.0588 * (100 * letters / words) - 0.296 * (100 * sentences / words) - 15.8

# print messages
if 1 <= grade < 16:
    print(f"Grade {round(grade)}")
elif 16 <= grade:
    print("Grade 16+")
else:
    print("Before Grade 1")