#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <string.h>

#define MAX 40'000'00
#define MIN 2
#define SIZE MAX - MIN

bool sieve_tab[(int) sqrt(MAX)] = {true};
long long primes_in_range[MAX - MIN];

int find_primes_sieve_dynamic(long long from, long long to) {
    int sqrt_max = (int) sqrt(to);
    for(int i = 0; i < sqrt_max - 1; i++) {
        if(!sieve_tab[i]) continue;
        int num = i + 2;
        for(int j = i + num; j < sqrt_max; j += num) {
            sieve_tab[j] = false;
        }
    }
    for(int i = 0; i < sqrt_max; i++) {
        if(sieve_tab[i]) {
            printf("%d\n", i + 2);
        } 
    }
    return 0;
}

int main() {
    memset(sieve_tab, true, sizeof(sieve_tab));
    find_primes_sieve_dynamic(MIN, MAX);
}

