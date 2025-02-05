#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>  // for uint64_t, etc.

// Sieve of Eratosthenes: return a list of primes up to 'limit'
std::vector<int> sieve_of_eratosthenes(int limit) {
    std::vector<bool> is_prime(limit + 1, true);
    is_prime[0] = false;
    is_prime[1] = false;
    for(int i = 2; i * i <= limit; i++){
        if(is_prime[i]){
            for(int j = i * i; j <= limit; j += i){
                is_prime[j] = false;
            }
        }
    }
    std::vector<int> primes;
    primes.reserve(limit / 2);
    for(int i = 2; i <= limit; i++){
        if(is_prime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Compute tau(n!) = product_{p <= n}(exponent_p + 1), where exponent_p = sum_{k>=1} floor(n / p^k).
// Returns that product in a 64-bit unsigned. Beware of overflow if n is large.
uint64_t tau_factorial(int n, const std::vector<int>& primes){
    uint64_t tau_val = 1ULL;

    for(int p : primes){
        if(p > n) {
            break;
        }
        // sum_{k=1..∞} floor(n / p^k)
        int exp_p = 0;
        long long power = p; 
        while(power <= n){
            exp_p += n / power;
            power *= p;
        }

        // multiply tau_val by (exp_p+1)
        // in Python, this can get arbitrarily large; here we can overflow.
        // you might add checks if (tau_val > ULLONG_MAX / (exp_p+1)) ...
        tau_val *= static_cast<uint64_t>(exp_p + 1);
    }
    return tau_val;
}

// Naive: returns the number of positive divisors of x in O(sqrt(x)) time
uint64_t numdiv(uint64_t x){
    uint64_t count = 0;
    uint64_t i = 1;
    while(i * i <= x){
        if(x % i == 0){
            if(i * i == x) {
                count += 1; // perfect square
            } else {
                count += 2; // i and x/i
            }
        }
        i++;
    }
    return count;
}

// Check if x is a power of 2 (and not zero)
bool is_power_of_2(uint64_t x){
    // x & (x-1) == 0 if x is 0 or a power of 2
    // also require x != 0
    return x != 0 && ( (x & (x - 1)) == 0 );
}

// fast_f: 
//  1) Compute m = tau_factorial(n) quickly
//  2) Repeatedly apply numdiv(m) until m is a power of 2
//  3) If that power == 1, return n; else n++ and repeat
uint64_t fast_f(int n, const std::vector<int>& primes){
    while(true){
        uint64_t m = tau_factorial(n, primes);

        // keep applying numdiv until m becomes a power of 2
        while(!is_power_of_2(m)){
            m = numdiv(m);
        }

        if(m == 1ULL){
            return n; // we found the n that collapses to 1
        }
        n++;
    }
}

// fast_g:
//  Compute tau_factorial(n), then repeatedly apply numdiv(m) until m is a power of 2.
//  Return log2(m). In Python code: "m.bit_length() - 1" if m is a power of 2.
uint64_t fast_g(int n, const std::vector<int>& primes){
    uint64_t m = tau_factorial(n, primes);
    while(!is_power_of_2(m)){
        m = numdiv(m);
    }
    // now m is a power of 2, so log2(m) = index of highest set bit
    // you can use built-in GCC/Clang function __builtin_ctzll or something similar
    // but for portability, do a loop or log2() if you want.
    // For a quick approach:
    int exponent = 0;
    while(m > 1){
        m >>= 1;
        exponent++;
    }
    return exponent;
}

int main(){
    // If you only go up to n=2000, we do sieve_of_eratosthenes(2000), etc.
    int max_n = 2000;
    std::vector<int> primes = sieve_of_eratosthenes(max_n);

    // Example usage:
    std::cout << "fast_f(1) = " << fast_f(1, primes) << "\n";
    std::cout << "fast_g(1) = " << fast_g(1, primes) << "\n";

    std::cout << R"(
    This code:
      - Precomputes primes up to 'max_n' using a sieve.
      - Computes tau(n!) by summing prime exponents for n! -> product(exponent+1).
      - fast_f(n): Repeats applying numdiv(...) until we get a power of 2. If it's 1, returns n, else increments n.
      - fast_g(n): Does the same repeated steps, but returns log2(...) of the final power of 2.

    WARNING: Potential overflow for large n or large tau(n!).
    Use a big-integer approach for truly large n.
    )" << std::endl;

    return 0;
}
