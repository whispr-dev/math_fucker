#include <iostream>
#include <vector>
#include <bitset>
#include <cstdint>

std::vector<int> sieve_of_eratosthenes(int limit) {
    std::bitset<2001> is_prime;
    is_prime.set();
    is_prime[0] = is_prime[1] = 0;
    std::vector<int> primes;
    primes.reserve(limit/2);
    
    for(int i = 2; i * i <= limit; i++)
        if(is_prime[i])
            for(int j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
    
    for(int i = 2; i <= limit; i++)
        if(is_prime[i])
            primes.push_back(i);
            
    return primes;
}

inline uint64_t tau_factorial(int n, const std::vector<int>& primes) {
    uint64_t tau_val = 1;
    for(int p : primes) {
        if(p > n) break;
        int exp_p = 0;
        uint64_t power = p;
        while(power <= n) {
            exp_p += n / power;
            if(power > n/p) break;
            power *= p;
        }
        tau_val *= (exp_p + 1);
    }
    return tau_val;
}

inline uint64_t numdiv(uint64_t x) {
    uint64_t count = 0;
    for(uint64_t i = 1; i * i <= x; i++)
        if(x % i == 0)
            count += (i * i == x) ? 1 : 2;
    return count;
}

constexpr bool is_power_of_2(uint64_t x) {
    return x && !(x & (x - 1));
}

uint64_t fast_f(int n, const std::vector<int>& primes) {
    while(true) {
        uint64_t m = tau_factorial(n, primes);
        while(!is_power_of_2(m)) m = numdiv(m);
        if(m == 1) return n;
        n++;
    }
}

uint64_t fast_g(int n, const std::vector<int>& primes) {
    uint64_t m = tau_factorial(n, primes);
    while(!is_power_of_2(m)) m = numdiv(m);
    return 63 - __builtin_clzll(m);
}

int main() {
    std::ios::sync_with_stdio(false);
    constexpr int max_n = 2000;
    const auto primes = sieve_of_eratosthenes(max_n);
    
    std::cout << "fast_f(1) = " << fast_f(1, primes) << '\n'
              << "fast_g(1) = " << fast_g(1, primes) << '\n';
    return 0;
}