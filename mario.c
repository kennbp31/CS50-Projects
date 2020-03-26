#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h = get_int("Height: "); //ultimate size of the pyramid
    int t = h; //used to track which iteration we are in. 

    while (t > 0)
    {
       for (int b = 0; b < (t-1); b++)
            {
                printf(" ");
            }
        for (int w = (t-1); w < h; w++)
            {
                printf("#");
            }
        printf("\n");
        t = t-1;
    }
    printf("\n");
}
