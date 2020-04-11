from cs50 import get_int
from cs50 import get_string

while True:
    h = get_int("Height: ")
    if h > 0 and h < 9:
        break

t = h

while (t > 0):
    for b in range(t-1):
        print(" ", end="")
        
    w = (t - 1)
    
    while (w < h):
        print("#", end="")
        w += 1
    
    print()
    t -= 1
        
        