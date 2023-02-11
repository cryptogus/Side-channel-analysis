from collections import defaultdict

def dup():


    fp = open("./CTF-7-Fault-message.txt", 'r')

    dd = defaultdict(int)

    for _ in range(0, 2046):
        a = fp.readline().strip()
        dd[a] += 1
    return dd

#dd = dup().values()
#print(dd)
