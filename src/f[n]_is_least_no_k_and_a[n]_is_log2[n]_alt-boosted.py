import math
import numba

@numba.njit
def valuation_2_numba(x):
    v = 0
    while x > 0 and (x % 2 == 0):
        x //= 2
        v += 1
    return v

@numba.njit
def numdiv_numba(x):
    count = 0
    i = 1
    while i * i <= x:
        if x % i == 0:
            count += 1 if i * i == x else 2
        i += 1
    return count

@numba.njit
def is_power_of_2_numba(x):
    return x != 0 and (x & (x - 1)) == 0

@numba.njit
def f_numba(n):
    while True:
        # factorial in a loop so it's compiled by numba
        m = 1
        for i in range(1, n + 1):
            m *= i

        while not is_power_of_2_numba(m):
            m = numdiv_numba(m)

        if m == 1:
            return n
        n += 1

def main():
    # The first call to f_numba(3) will trigger Numba's JIT compilation
    result = f_numba(5)
    print("f_numba(5) =", result)

if __name__ == "__main__":
    main()
