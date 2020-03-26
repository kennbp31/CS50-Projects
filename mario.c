#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h = get_int("Height: \n"); //ultimate size of the pyramid
    int t = h; //used to track which iteration we are in. 

    while (t > 0)
    {
      
        for (int w = 0; w < (t-1); w++)
            {
                printf("#");
            }
        printf("\n");
        t = t-1;
    }
    printf("\n");
}
