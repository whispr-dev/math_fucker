#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>      // for std::sqrt, std::floor
#include <cstdint>

// We assume limit = 2000, so a bitset<2001> is fine.
std::vector<int> sieve_of_eratosthenes(int limit) {
    std::bitset<2001> is_prime;
    is_prime.set();
    is_prime[0] = is_prime[1] = 0;
    std::vector<int> primes;
    primes.reserve(limit / 2);

    // Optional: int sq = (int)std::sqrt((long double)limit);
    for(int i = 2; i * i <= limit; i++) {
        if(is_prime[i]) {
            for(int j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    for(int i = 2; i <= limit; i++) {
        if(is_prime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

// inline or static inline
inline uint64_t tau_factorial(int n, const std::vector<int>& primes) {
    uint64_t tau_val = 1ULL;
    for(int p : primes) {
        if(p > n) break;
        int exp_p = 0;
        uint64_t power = p;
        while(power <= (uint64_t)n) {
            exp_p += n / power;
            power *= p;
        }
        tau_val *= (exp_p + 1);
    }
    return tau_val;
}

// sqrt-based approach
inline uint64_t numdiv(uint64_t x) {
    uint64_t count = 0;
    // single sqrt for the loop bound
    uint64_t end = static_cast<uint64_t>(std::floor(std::sqrt((long double)x)));
    for(uint64_t i = 1; i <= end; i++) {
        if(x % i == 0) {
            count += (i * i == x) ? 1ULL : 2ULL;
        }
    }
    return count;
}

constexpr bool is_power_of_2(uint64_t x) {
    return x && !(x & (x - 1));
}

uint64_t fast_f(int n, const std::vector<int>& primes) {
    while(true) {
        uint64_t m = tau_factorial(n, primes);
        while(!is_power_of_2(m)) {
            m = numdiv(m);
        }
        if(m == 1ULL) {
            return n;
        }
        n++;
    }
}

uint64_t fast_g(int n, const std::vector<int>& primes) {
    uint64_t m = tau_factorial(n, primes);
    while(!is_power_of_2(m)) {
        m = numdiv(m);
        if(m == 1ULL) {
            // early exit: log2(1) = 0
            return 0ULL;
        }
    }
    // built-in for log2 of a power of two:
    // 63 - __builtin_clzll(m)
    return 63 - __builtin_clzll(m);
}

int main() {
    std::ios::sync_with_stdio(false);
    // if you're also using std::cin heavily, do:
    // std::cin.tie(nullptr);

    constexpr int max_n = 2000;
    auto primes = sieve_of_eratosthenes(max_n);

    std::cout << "fast_f(1) = " << fast_f(1, primes) << '\n'
              << "fast_g(1) = " << fast_g(1, primes) << '\n';

    return 0;
}
