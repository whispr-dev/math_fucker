import math

def valuation_2(x: int) -> int:
    """Return v where 2^v divides x."""
    v = 0
    while x > 0 and (x % 2 == 0):
        x //= 2
        v += 1
    return v

def numdiv(x: int) -> int:
    """Returns the number of positive divisors of x."""
    count = 0
    i = 1
    while i * i <= x:
        if x % i == 0:
            # if i*i == x, that's one divisor, else two
            count += 1 if i * i == x else 2
        i += 1
    return count

def f(n: int) -> int:
    """
    1) Sets m = n!.
    2) Repeatedly replaces m with numdiv(m) until m is a power of 2.
    3) If m ends up being 1, return n; otherwise n++, repeat forever.
    """
    while True:
        m = math.factorial(n)

        # Keep applying numdiv until m is a power of 2
        while (1 << valuation_2(m)) != m:
            m = numdiv(m)

        if m == 1:
            return n
        n += 1  # Keep searching if it's not 1

def a(n: int) -> int:  # i.e., it returns the exponent of 2 (valuation_2) once factorial(n) becomes a power of 2.

    m = math.factorial(n)
    while (1 << valuation_2(m)) != m:
        m = numdiv(m)
    return valuation_2(m)

# Example usage
if __name__ == "__main__":
    print("f(3) =", f(3))    # This might loop until it finds the first n that leads to m=1
    print("a(3) =", a(3))  # This returns the exponent of 2 at the end of the chain for 10

    print("where f(n) is Starting at 𝑛, repeatedly computing 𝑛! and applying the number-of-divisors function to that factorial until the result is a power of 2. If the power of 2 is 1, 𝑓(𝑛) returns 𝑛. Otherwise, it increments 𝑛 by 1 and tries again, effectively searching for the first integer ≥ 𝑛 whose factorial eventually collapses to 1 under repeated divisor counting. Computing 𝑛!, then repeatedly applying the number-of-divisors function until the result becomes a power of 2. Once it is a power of 2, a(𝑛) returns the exponent of that power—i.e., log2(final_result)).")
