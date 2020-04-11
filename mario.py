from cs50 import get_int
from cs50 import get_string

while True:
    h = get_int("Height: ")
    if h > 0 and h < 9:
        break


for t in range(h, 0, -1):

    for b in range(t-1):
        print(" ", end="")
        
    for w in range(t-1, h, 1):
        print("#", end="")
        
    print("  ", end="")
    
    for w in range(t-1, h, 1):
        print("#", end="")
    
    print()
