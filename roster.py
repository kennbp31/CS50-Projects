import csv
import cs50
import sys


# make sure command line args were used
if len(sys.argv) != 2:
        print("Missing Command Line Argument!")
        sys.exit(1)


# us the cs50 library to provide link and controls of new DB
db = cs50.SQL("sqlite:///students.db")


# load all data into a buffer/dictionary when the argument is the house, ordered by last name
roster = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last", sys.argv[1] )


# print each row formatted to the screen, check for the absence of a middle name
for row in roster:
    if row["middle"] == None:
        print(row["first"], row["last"]+ ", born", row["birth"])
    elif row["middle"] != None:
        print(row["first"], row["middle"], row["last"]+ ", born", row["birth"])
