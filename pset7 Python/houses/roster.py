from cs50 import SQL
from csv import reader
from sys import argv

# check for correct number of arguments
if len(argv) != 2:
    print('Usage: python import.py characters.csv')
    exit(1)

# open db set it to variable
db = SQL("sqlite:///students.db")

# get all the students as dictionary
students = db.execute('SELECT * FROM students WHERE house = (?) ORDER BY last', argv[1])

for student in students:
    if student['middle'] != None:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['last']}, born {student['birth']}")