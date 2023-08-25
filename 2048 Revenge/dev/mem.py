from pwn import*
l = [pow(2, i) for i in range(1, 37)]
for i in l:
    print(i)