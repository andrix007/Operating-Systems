import numpy as np

# Given data
n = 5
m = 5
t = np.array([6, 17, 9, 9, 7])
A = np.array([
    [1, 5, 3, 1, 1],
    [0, 2, 1, 1, 1],
    [0, 7, 1, 2, 1],
    [3, 1, 1, 1, 0],
    [1, 2, 3, 2, 1]
])
M = np.array([
    [2, 5, 3, 3, 2],
    [3, 5, 8, 9, 1],
    [4, 9, 4, 9, 2],
    [6, 1, 4, 5, 5],
    [1, 2, 3, 4, 5]
])

# Calculate available resources
Avail = t - np.sum(A, axis=0)

# Determine if the system is in a safe state
SafeSeq = []
Work = Avail.copy()
Finish = [False] * n

while len(SafeSeq) < n:
    found = False
    #printing safe sequence, work, finish, Available, and M
    print("Safe sequence = ", SafeSeq)
    print("Work = ", Work)
    print("Finish = ", Finish)
    print("Available = ", Avail)
    print()

    for i in range(n):
        if not Finish[i] and np.all(M[i] - A[i] <= Work):
            SafeSeq.append(i)
            Work += A[i]
            Finish[i] = True
            found = True
            break
    if not found:
        break

is_safe = len(SafeSeq) == n
is_safe, SafeSeq if is_safe else None

print(is_safe)