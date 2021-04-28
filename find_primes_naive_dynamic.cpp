#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

#define MAX 40'000'00
#define MIN 2
#define SIZE MAX - MIN

long long primes[(int)(SIZE / 2)];
long long primes_count = 0;

int find_primes_naive_dynamic(long long from, long long to) {
    if(to < 2 || to <= from) return 0;

    ++primes_count;
    primes[0] = 2;

    #pragma omp parallel for schedule(dynamic)
    for(long long i = 3; i < to; i += 2) {
        bool is_prime = true;
        long long last_num = sqrt((double) i);
        for(long long j = 3; j <= last_num; j += 2) {
            if((i % j) == 0) {
                is_prime = false;
                break;
            }
        }
        #pragma omp critical
        {
            if(is_prime) {
                primes[primes_count++] = i;
            }
        }
    }
    return primes_count;
}

int main() {
    omp_set_num_threads(8);
    printf("%d\n", find_primes_naive_dynamic(MIN, MAX));
    // for(int i = 0; i < primes_count; i++) {
    //     printf("%lld\n", primes[i]);
    // }
}