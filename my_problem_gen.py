import sys
from random import randint
if(len(sys.argv)<4):
    print("ERROR: Too few Arguments")
    sys.exit(1)
N = int(sys.argv[1])
K1 = int(sys.argv[2])
K2 = int(sys.argv[3])
E = (K1 * (K1 - 1)) // 2 + (K2 * (K2 - 1)) // 2
E = randint(E, (N * (N - 1)) // 2)
G = {}
for i in range(1, N+1):
    G[i] = []
num_edges = 0
for i in range(0, K1):
    for j in range(i+1, K1):
        G[i+1].append(j+1)
        num_edges += 1
for i in range(K1, K1 + K2):
    for j in range(i+1, K1 + K2):
        G[i+1].append(j+1)
        num_edges += 1
while num_edges < E:
    v1 = randint(1, N-1)
    v2 = randint(v1 + 1, N)
    if v2 in G[v1]:
        continue
    else:
        G[v1].append(v2)
        num_edges += 1
fp = open('test.graph','w')
fp.write('%d %d %d %d\n' %(N,E, K1, K2))
for i in range(1,N+1):
    for v in G[i]:
        fp.write('%d %d\n' %(i,v))
fp.close()
