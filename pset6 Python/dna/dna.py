from csv import reader, DictReader
from sys import argv

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# extract only the first line of csv containing dna sequence list
# store it into array
# pop off names
# break the loop after 1 iteration
with open(argv[1]) as peopleFile:
    people = reader(peopleFile)
    for row in people:
        dnaSequences = row
        dnaSequences.pop(0)
        break

# assoc array for counting
sequences = {}

# copy dna sequences as keys to assoc array and set values to 0
for i in dnaSequences:
    sequences[i] = 0

# open and read suspect dna sequence
with open(argv[2]) as dnaFile:
    # gets an object
    dnaReader = reader(dnaFile)
    # iterate the file
    for row in dnaReader:
        # get array filled with one long string
        dnaList = row

# convert array to string
emptyString = ""
dnaString = emptyString.join(dnaList)

# for every dnaMArker at top of the table
for dnaMarker in sequences:
    dnaMarkerRepetition = 0
    n = 1
    # look for a dna marker, if found search for longer
    while dnaMarker * n in dnaString:
        n += 1
    # check if longest
    if n - 1 > dnaMarkerRepetition:
        dnaMarkerRepetition = n - 1
    # save to dict
    sequences[dnaMarker] = dnaMarkerRepetition

with open(argv[1], newline='') as peopleFile:
    # open as dict
    people = DictReader(peopleFile)
    for person in people:
        match = 0
        # compare sequences with people
        for dnaMarker in sequences:
            if sequences[dnaMarker] == int(person[dnaMarker]):
                match += 1
        # if number matches amount of dnaMarkers in file
        if match == len(sequences):
            print(person['name'])
            exit(0)
    print("No match")