#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2) //check that a command line argument was provided
    {
        printf("missing command-line argument\n");
        return 1;
    }

    int length = strlen(argv[1]); //count length of string provided in command line
    string input = argv[1]; //prepare input string for manipulation
    //  printf("%i total length\n", length);//letter count check
    int letters = 0;//variable used to count the number of good characters
    string lower = input;
    string upper = input;
    int dups = 0;


    //convert input cipher to upper and lower case

    for (int i = 0; i < length; i++) //loop to ensure that the characters are not numbers or other bull
    {
         if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
        {
            letters++;
        }
    }

   // printf("%i Total letters\n", letters); //Character Check

    if ((length != letters) || (letters != 26)) //compare string length and number of actual letters provided
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    
    for (int i = 0; i < length; i++)// duplicate letter check
    {
        dups = 0;
        for (int l = 0; l < length; l++)
        {
            if (input[i] == input[l])
            {
               dups++; 
            }
            
        }
        if (dups > 1)
            {
                printf("Key must not contain more than 1 of the same letter\n");
                return 1;
            }
        //copy each letter into 2 separate arrays
        //check the first letter and compare it to each letter
        //if it matches one of the letters count 1
        //if if count > 1 at end of loop, return 1
    }

    //printf("Good Key\n");
    string plain = get_string("plaintext: ");//collect text to convert
    int ln = strlen(plain);
    string standard = "abcdefghijklmnopqrstuvwxyz";
    string ucasestnd = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int cletter = 0;
    int A = 'A';
    int Z = 'Z';
    int al = 'a';
    int zl = 'z';
    int plainint = 0;

    //printf("%i\n", A);

    printf("ciphertext: ");


    for (int i = 0; i < ln; i++)
    {
        cletter = 0;
        plainint = 0;
        plainint = plain[i];
       // printf("%i\n", plainint);

        if (plainint >= al && plainint <= zl)
        //
        {
            while (plain[i] != standard[cletter])
                {
                 cletter++;
                }

            for (int j = 0; lower[j]; j++)
            {
                lower[j] = tolower(lower[j]);
             }

            printf("%c", lower[cletter]);
        }
        else if (plainint >= A && plainint <= Z)
        {
            while (plain[i] != ucasestnd[cletter])
                {
                 cletter++;
                }
            for (int k = 0; upper[k]; k++)
            {
                upper[k] = toupper(upper[k]);
              }

            printf("%c", upper[cletter]);
        }

        else
        {
        printf("%c", plain[i]);
        }
    }
     printf("\n");
     return 0;
}