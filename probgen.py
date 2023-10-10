import sys
import random
if(len(sys.argv)<4):
    print("ERROR: Too few Arguments")
    sys.exit(1)
N = int(sys.argv[1])
K1 = int(sys.argv[2])
K2 = int(sys.argv[3])
N2 = int(max(N-1,(K1*(K1-1))/2 + (K2*(K2-1))/2))
E = random.randint(N2,N*(N-1)/2)
G = {}
choices = list(range(1,N))
random.shuffle(choices)
L1 = choices[0:K1]
L2 = choices[K1:K1+K2]
fp = open('test.graph','w')
f = 0
# fp.write('%d %d %d %d\n' %(N,E, K1, K2))
print(*sorted(L1))
if (K2 > 0):
    print(*sorted(L2))
M = []
for i in range(len(L1)):
    for j in range(i+1,len(L1)):
            M.append((L1[j],L1[i]))
            f+=1
for i in range(len(L2)):
    for j in range(i+1,len(L2)):
        M.append((L2[j],L2[i]))
        f+=1
for i in range(E-f):
    v1 = random.randint(1,N)
    v2 = random.randint(1,N)
    M.append((v1,v2))
M = set(M)
M = list(M)
if (K2 > 0):
    fp.write('%d %d %d %d\n' %(N,len(M), K1, K2))
else:
    fp.write('%d %d\n' %(N, len(M)))
for i in range(len(M)):
    fp.write('%d %d\n' %(M[i][0],M[i][1]))
fp.close()
