from cs50 import SQL
from csv import reader
from sys import argv

# check for correct number of arguments
if len(argv) != 2:
    print('Usage: python import.py characters.csv')
    exit(1)

# open db set it to variable
db = SQL("sqlite:///students.db")

# open csv, get list of arrays, iterate to get each individul array
with open(argv[1]) as file:
    students = reader(file)
    for student in students:
        # get rid of line containing markers
        if student[0] == 'name':
            continue

        # get first, middle, last names by spliting each arrays first member
        name = student[0].split()

        # insert data into db, add NULL(None) incase there is no middleName
        if len(name) < 3:
            db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)',
                       name[0], None, name[1], student[1], student[2])
        else:
            db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)',
                       name[0], name[1], name[2], student[1], student[2])