#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float owed = 0;//define main float
    while (owed <= 0)//get user input, check to ensure it is valid float
    {
        owed = get_float("Change Owed: ");
    }

    int total = round(owed * 100);//conver to an easier amount to work with
    int quarters = (total / 25);
    int dimes = ((total - (quarters * 25)) / 10);
    int nickels = ((total - ((quarters * 25) + (dimes * 10))) / 5);
    int pennies = (total - ((quarters * 25) + (dimes * 10) + (nickels * 5)));
    int coins = quarters + dimes + nickels + pennies;
   
    printf("%i\n", coins);

}
