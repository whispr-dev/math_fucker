#include <iostream>
#include <cmath>
#include <limits>

// Compute n! in 64-bit integer (caution: overflow for n>20)
long long factorial(int n) {
    long long result = 1;
    for(int i = 1; i <= n; i++) {
        result *= i;
        // Optional: check for overflow if you want 
        // if (result < 0) { /* handle error */ }
    }
    return result;
}

// Return v where 2^v divides x
int valuation_2(long long x) {
    int v = 0;
    while(x > 0 && (x % 2 == 0)) {
        x /= 2;
        v++;
    }
    return v;
}

// Return the number of positive divisors of x
long long numdiv(long long x) {
    long long count = 0;
    for(long long i = 1; i * i <= x; i++) {
        if(x % i == 0) {
            // if i*i == x, it's one divisor, else two
            count += (i * i == x) ? 1 : 2;
        }
    }
    return count;
}

// The function f(n):
//  1) m = n!
//  2) Repeatedly do m = numdiv(m) until m is a power of 2
//  3) If that power of 2 == 1, return n; otherwise n++ and repeat
int f_func(int n) {
    while(true) {
        long long m = factorial(n);

        // While m isn't a power of 2 -> keep applying numdiv
        // Checking "while ((1 << valuation_2(m)) != m)"
        // But watch out if valuation_2(m) > 63 => shifting 1LL is risky
        // We'll just do it, but for big m or big n it might be invalid
        while(true) {
            long long shiftVal = (1LL << valuation_2(m));
            if(shiftVal == m) {
                break; // Now m is a power of 2
            }
            m = numdiv(m);
        }

        if(m == 1) {
            return n;
        }
        n++;
    }
}

// The function a(n):
//  Compute n!, repeatedly apply numdiv until m is a power of 2, then return valuation_2(m).
int a_func(int n) {
    long long m = factorial(n);
    while(true) {
        long long shiftVal = (1LL << valuation_2(m));
        if(shiftVal == m) {
            break; // m is a power of 2
        }
        m = numdiv(m);
    }
    return valuation_2(m);
}

int main() {
    // Example usage: 
    //   f_func(3) => might loop until it finds the first n s.t. n! repeated divisor counting => 1
    //   a_func(3) => exponent of 2 for the final power of 2 after repeated divisor steps.
    std::cout << "f(1) = " << f_func(1) << "\n";
    std::cout << "a(1) = " << a_func(1) << "\n";

    std::cout 
        << "where f(n) is Starting at n, repeatedly computing n! and applying the number-of-divisors function\n"
           "to that factorial until the result is a power of 2. If the power of 2 is 1, f(n) returns n.\n"
           "Otherwise, it increments n by 1 and tries again, effectively searching for the first integer\n"
           ">= n whose factorial eventually collapses to 1 under repeated divisor counting.\n"
           "Once it is a power of 2, a(n) returns the exponent of that power—i.e., log2(final_result).\n";

    return 0;
}
