#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>


int spaces(string in, int len);

int main(void)
{
    string input = get_string("Text: "); //ask user for input
    int length = strlen(input);  //constant for total characters
    int letters = 0;
    int sentences = 0;
    int words = spaces(input, length);
    
    
    for (int l = 0; l < length; l++ )
    {
        if ((input[l] >= 'a' && input[l] <= 'z') || (input[l] >= 'A' && input[l] <= 'Z'))
        {
            letters++;
        }
    }
    
    for (int s = 0; s < length; s++ )
    {
        if (input[s] == '.' || input[s] == '?' || input[s] == '!')
        {
            sentences++;
        }
    }
    
    //letters / words * 100 = letters per 100 words
    //sentences / words * 100 = sentences per 100 words
    // final formula: 0.0588 * L - 0.296 * S - 15.8
    float grade = ((0.0588 * (((float) letters / (float) words) * 100))  - (0.296 * (((float) sentences / (float) words) * 100)) - 15.8);
    
   // printf("%i letter(s)\n", letters);
   // printf("%i word(s)\n", words);
   // printf("%i sentence(s)\n", sentences);
    
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    
    else
    {
         printf("Grade %.0f\n", round(grade));
    }
   

}



int spaces(string in, int len)// Function to determine the number of words!!
{
    int s = 1;
    for (int i = 0; i < len; i++ )
    {
        if (in[i] == ' ')
        {
            s++;
        }
    }
    return s;
}
    
 
        
    