import re
import sys
import time
import csv

def main():
    
    #make sure command line args were used
    if len(sys.argv) != 3:
        print("Missing Command Line Arguments!")
        sys.exit(1)

    print(f"{sys.argv[1]} {sys.argv[2]}")
    
    # Function call list
    database = load_databases()
    sequence = load_sequences()
    matches = analyze_sequences(database, sequence)
    find_matches(matches, database)

#Counts the patterns in the DNA sequence provided in the command line
def analyze_sequences(database, sequence):

    #define variables
    matches = [] 
    seq_count = database['ncol']
    
    
    #Process to count the number of seq repetitions in the txt files
    for s in range (seq_count):
        matches.append(0)
        cur_seq = database['seqs'][s]
        for i in range(len(sequence)):
            if sequence[i:i + len(cur_seq)] == cur_seq: # check that the sequence exists
                count = 0
            
                for j in range(i, len(sequence), len(cur_seq)): # start a new loop that iterates based on size of sequence
                    if (sequence[j:j + len(cur_seq)] == cur_seq): # for each match, count
                        count += 1

                        if (matches[s] < count): #if the new count is greater than the match total, overwrite the match total
                              matches[s] = count
                            
                    elif (sequence[j:j + len(cur_seq)] != cur_seq): #set i to j to continue the search for other repeat sequences
                        i = j
                        #print(s1_match, "- total matches thus far")
                        break
            
    return(matches)



# load the CSV into memory

def load_databases():
    
    with open(sys.argv[1], mode='r') as csv_file:
        database = csv.reader(csv_file)
        
        #set topline of CSV to seq_list array
        first_line = csv_file.readline()
        ncol = (first_line.count(',') + 1) - 1 #count number of collumns
        seq_list = [] #create an array/list to put the sequences
        
        #create dictionary for peoples names and dna
        dna = []
        
        csv_file.seek(0) #return back to the beginning of the CSV
        for i,col in enumerate(database): #navigate through the file, can probably do this better
            if i == 0:
                for j in range(1, ncol + 1, 1):
                    seq_list.append(col[j]) #add each collumn to the list
            #for the rows that ARE not headers, create a 2 dimensional list, with their specific information.
            elif i > 0:
                dna.append([])
                for k in range(ncol+1):
                    dna[i-1].append(col[k])
    
    #create a dictionary to pass back info to main
    seq_tup = seq_list
    dna_tup = dna
    
    r = dict();
    r['seqs'] = seq_tup
    r['ncol'] = ncol
    r['dna'] = dna_tup
    return r 
                    

# load sequences into memory
def load_sequences():
   
    seq = open(sys.argv[2], mode='r')
    #print(seq.read())
    return (seq.read())


# Compare the matches list to list of people's sequences
def find_matches(matches, database):

    for row in range(len(database['dna'])):
        for col in range(len(matches)):
            if int(database['dna'][row][col + 1]) != int(matches[col]):
                break
            if int(database['dna'][row][col + 1]) == int(matches[col]):
                if col + 1 == database['ncol']:
                    print(database['dna'][row][0])
                    return()
        if row + 1 == len(database['dna']):
            print("No match")
            return()
    
main()