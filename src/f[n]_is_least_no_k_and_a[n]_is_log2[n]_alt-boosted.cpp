#include <iostream>
#include <cstdint>

// Returns v where 2^v divides x
int valuation_2_numba(uint64_t x) {
    int v = 0;
    while(x > 0 && (x % 2 == 0)) {
        x /= 2;
        v++;
    }
    return v;
}

// Returns the number of positive divisors of x
uint64_t numdiv_numba(uint64_t x) {
    uint64_t count = 0;
    uint64_t i = 1;
    while(i * i <= x) {
        if(x % i == 0) {
            // if i*i == x, it's one divisor, else two
            count += (i * i == x) ? 1ULL : 2ULL;
        }
        i++;
    }
    return count;
}

// Check if x is a power of 2 (and x != 0)
bool is_power_of_2_numba(uint64_t x) {
    return x != 0 && ((x & (x - 1)) == 0);
}

// f_numba(n):
//   1) compute n! in a loop (beware overflow if n>20)
//   2) repeatedly apply numdiv_numba(...) until it becomes a power of 2
//   3) if it ends up 1, return n, else increment n and repeat
uint64_t f_numba(int n) {
    while(true) {
        // factorial in a loop
        uint64_t m = 1ULL;
        for(int i = 1; i <= n; i++){
            m *= i;
        }

        // while it's not a power of 2, apply numdiv_numba
        while(!is_power_of_2_numba(m)) {
            m = numdiv_numba(m);
        }

        if(m == 1ULL) {
            return n;
        }
        n++;
    }
}

// g_numba(n):
//   1) compute n!
//   2) repeatedly apply numdiv_numba(...) until it becomes a power of 2
//   3) return log2(m), i.e., the exponent (if m == 2^e)
int g_numba(int n) {
    // Step 1: factorial
    uint64_t m = 1ULL;
    for(int i = 1; i <= n; i++){
        m *= i;
    }

    // Step 2: repeatedly apply numdiv until m is a power of 2
    while(!is_power_of_2_numba(m)) {
        m = numdiv_numba(m);
    }

    // Step 3: return exponent = log2(m)
    // You can do a loop, or use built-in bit-twiddling
    int exponent = 0;
    while(m > 1ULL) {
        m >>= 1;
        exponent++;
    }
    return exponent;
}

int main() {
    // Example usage
    uint64_t fResult = f_numba(1);
    int gResult = g_numba(1);

    std::cout << "f_numba(1) = " << fResult << "\n";
    std::cout << "g_numba(1) = " << gResult << "\n";

    std::cout << R"(

This C++ code provides both:
  - f_numba(n): the "search" method that increments n if final power of 2 != 1.
  - g_numba(n): simply returns log2(...) of the final power of 2 after repeated divisor steps on n!.

WARNING: 
  - factorial overflows 64-bit if n>20.
  - for bigger n, you'd want a big integer or a different approach (like computing tau(n!) via prime exponents).
)" << std::endl;

    return 0;
}
