// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 27;
int total_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char * alphabet = "abcdefghijklmnopqrstuvwxyz";
    int i = 0;
    const char lowdic = tolower(word[0]);
    node *tmp = table[i];

    while (i < N)
    {
        if (lowdic == alphabet[i])
        {
            
            //set temp = the table start
            tmp = table[i];
            
            //iterate through the hash table to find the last record
            if (tmp == NULL)
            {
               return false; 
            }
            
            while (tmp != NULL)
            {
                if (strcasecmp(tmp->word, word) == 0)
                {
                    //printf("MATCH! %s\n", tmp->word);
                    return true;
                }
                else
                {
                tmp = tmp->next;
                }
            }
           
            i++;
            
        }
        
        else
        {
            i++;
        }
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    char * alphabet = "abcdefghijklmnopqrstuvwxyz";
    int i = 0;
    const char lowdic = tolower(word[0]);
    node *tmp = table[i];
 
    while (i < N)
    {
        if (lowdic == alphabet[i])
        {
            node *n = malloc(sizeof(node));
            
            if (n == NULL)
            {
                return 1;
            }
            
            //copy new word into last node of has table
            strcpy(n->word, word);
            n->next = NULL;
            total_words++;
            
            if (table[i] == NULL)
            {
                table[i] = n;
                break;
            }
            
            //set temp = the table start
            tmp = table[i];
            
            //iterate through the hash table to find the last record
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
           
            //add the newest word to the end of the hash table
            tmp->next = n;
           
            //temporary print to confirm we are hashing baby!
            //for (tmp = table[i] ; tmp != NULL; tmp = tmp->next)
            //{
                //printf("NODE! %s\n", tmp->word);
                
            //}
            break;
            
        }
        
        else
        {
            i++;
        }
    }
    
    return i;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary in temp dic
    FILE *dic = fopen(dictionary, "r");
    
    //ensure that it loaded correctly, otherwise abort
    if (dic == NULL)
    {
        fclose(dic);
        printf("Unable to open file\n");
        return 1;
    }
    
    //declare variables used to gather data from dictionary
    int c;
    char word[LENGTH + 1]= "";
   
    int i = 0;
    while ((c = fgetc(dic)) != EOF)
    {   
        //if the next character is not the LF "\n" copy that letter into the next character array
        if (c != 10)
        {
        word[i] = c;
        i++;
        }
        
        //if the current character IS LF, "\n", pass the word array into the hash function, clear out the word, start again.
        if (c == 10)
        {
            i++;
           // word[i] = '\0';
            hash(word);
            //printf("\n dictionary check %s", word);
            for (int j = 0; j < LENGTH + 1; j++)
            {
                word[j] = 0;
            }
            i = 0;
        }
    }
    
    
    
    //for each word, call the has function and pass it the word in the dictionary
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    char * alphabet = "abcdefghijklmnopqrstuvwxyz";
    int i = 0;
    node *tmp = table[i];
    node *cursor = table[i];
 
    while (i < N)
    {
        //set temp = the table start
        tmp = table[i];
        cursor = table[i];
            
        if (table[i] != NULL)//iterate through the hash table to find the last record
        {
            while (cursor->next != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            
            }
        }
        
        i++;
        free(cursor);
        free(tmp);
    }
    
    return true;
}
