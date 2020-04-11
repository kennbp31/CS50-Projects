from cs50 import get_float

while True:
    owed = get_float("Change Owed: ")
    if owed > 0:
        break

total = round(owed * 100)
quarters = int((total / 25))
dimes = int(((total - (quarters * 25)) / 10))
nickels = int(((total - ((quarters * 25) + (dimes * 10))) / 5))
pennies = int((total - ((quarters * 25) + (dimes * 10) + (nickels * 5))))
coins = quarters + dimes + nickels + pennies

print(round(coins))