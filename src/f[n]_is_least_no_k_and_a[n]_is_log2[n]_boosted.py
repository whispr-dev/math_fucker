import math
import sys
input_data = sys.stdin.read()  # if you read a lot

def sieve_of_eratosthenes(limit):
    """Return a list of primes up to 'limit'."""
    is_prime = [True]*(limit+1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5)+1):
        if is_prime[i]:
            for j in range(i*i, limit+1, i):
                is_prime[j] = False
    return [p for p in range(2, limit+1) if is_prime[p]]

def tau_factorial(n, primes):
    """
    Compute number of divisors of n! using the prime exponents formula.
    n! = Product_{p <= n} p^exponent_p, 
    so tau(n!) = Product_{p <= n} (exponent_p + 1).
    """
    tau_val = 1
    for p in primes:
        if p > n:
            break
        # sum_{k=1..∞} floor(n / p^k)
        exp_p = 0
        power = p
        while power <= n:
            exp_p += n // power
            power *= p
        tau_val *= (exp_p + 1)
    return tau_val

def numdiv(x: int) -> int:
    """Naive: returns the number of positive divisors of x in O(sqrt(x))."""
    count = 0
    i = 1
    while i * i <= x:
        if x % i == 0:
            count += 1 if (i * i == x) else 2
        i += 1
    return count

def is_power_of_2(x):
    return x and (x & (x - 1)) == 0  # classic bit-trick

def fast_f(n: int, primes) -> int:
    """
    Variation of your f(n) using prime-based tau(n!) for the first step:
    1) Compute m = tau(n!) quickly with prime factor exponents.
    2) Repeatedly apply numdiv(m) until m is a power of 2.
    3) If that power == 1, return n; else n++ and repeat.
    """
    while True:
        m = tau_factorial(n, primes)  # big speed-up vs computing n! then numdiv
        while not is_power_of_2(m):
            m = numdiv(m)
        if m == 1:
            return n
        n += 1  # keep searching if final power of 2 wasn't 1

def fast_g(n: int, primes) -> int:
    """
    Variation of your a(n) that returns log2 of final power-of-2
    after repeated divisor steps, but we do tau(n!) as the first step.
    """
    m = tau_factorial(n, primes)
    while not is_power_of_2(m):
        m = numdiv(m)
    # now m is some power of 2, so we can return exponent:
    # e.g. 'm.bit_length() - 1' is the same as log2(m) if m is a power of 2
    return m.bit_length() - 1

if __name__ == "__main__":
    # Precompute primes up to some maximum 'n' you care about.
    # If you only go up to n=10_000, do sieve_of_eratosthenes(10000), etc.
    max_n = 2000
    primes = sieve_of_eratosthenes(max_n)

    # Example usage:
    print("fast_f(1) =", fast_f(1, primes))
    print("fast_g(10) =", fast_g(10, primes))
