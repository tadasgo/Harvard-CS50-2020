from cs50 import get_int

while True:
    height = get_int("Please input height of pyramid (1 to 8, inclusive): ")
    if 1 <= height <= 8:
        break

for i in range(1, height + 1):
    spaces = height - i
    print(" " * spaces, end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)