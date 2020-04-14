import csv
import cs50
import sys

# make sure command line args were used
if len(sys.argv) != 2:
    print("Missing Command Line Argument!")
    sys.exit(1)

# create empty database and release the connection, will write over if already exists
open(f"students.db", "w").close()

# us the cs50 library to provide link and controls of new DB
db = cs50.SQL("sqlite:///students.db")

# create the table where the roster data will live.
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# load the CSV in a buffer for insertion to DB
with open(sys.argv[1], mode='r') as csv_file:
    students = csv.DictReader(csv_file)
   
   # Navigate through each row and insert it into the DB
    for row in students:
        
        # count spaces in each name for parsing
        list = row["name"].split()
        count = 0
        for i in row["name"]:
            if i == " ":
                count += 1
                
        # if they have first middle and last do this
        if count == 2:
            #print(list[0], list[1], list[2])
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", 
                    list[0], list[1], list[2], row["house"], row["birth"])
            
        # if they only have 2 names, import this. NOTE: None is NULL in SQL
        else:
            #print(list[0], list[1])
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", 
                    list[0], None, list[1], row["house"], row["birth"])
