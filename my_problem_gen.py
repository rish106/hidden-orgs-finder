import sys
from random import randint, shuffle


if (len(sys.argv) < 3) :
    print("ERROR: Too few Arguments")
    sys.exit(1)

N = int(sys.argv[1])
K1 = int(sys.argv[2])
K2 = 0

if (len(sys.argv) == 4):
    K2 = int(sys.argv[3])

if (K1 + K2 > N):
    print("Invalid arguments, cannot form clique(s) of this size")
    exit(1)

E = (K1 * (K1 - 1)) // 2 + (K2 * (K2 - 1)) // 2
# E = randint(E, (N * (N - 1)) // 2)
G = {}
V = list(range(1, N+1))

shuffle(V)

first_clique = V[0:K1].copy()
first_clique.sort()
print(f"Clique of size {K1}")
print(*first_clique)

if (K2 > 0):
    second_clique = V[K1:(K1+K2)].copy()
    second_clique.sort()
    print(f"Clique of size {K2}")
    print(*second_clique)

for i in range(1, N+1):
    G[i] = []
num_edges = 0

for i in range(0, K1):
    for j in range(i+1, K1):
        x = min(V[i], V[j])
        y = max(V[i], V[j])
        G[x].append(y)
        num_edges += 1

for i in range(K1, K1 + K2):
    for j in range(i+1, K1 + K2):
        x = min(V[i], V[j])
        y = max(V[i], V[j])
        G[x].append(y)
        num_edges += 1

while num_edges < E:
    v1 = randint(1, N-1)
    v2 = randint(v1 + 1, N)
    if v2 in G[v1]:
        continue
    else:
        G[v1].append(v2)
        num_edges += 1

edges = []
for i in V:
    for v in G[i]:
        edges.append((i, v))

shuffle(edges)

fp = open('test.graph','w')
fp.write('%d %d' %(N, E))
if (K2 != 0):
    fp.write(' %d %d' %(K1, K2))
fp.write('\n')
for edge in edges:
    fp.write('%d %d\n' %(edge[0], edge[1]))
fp.close()
