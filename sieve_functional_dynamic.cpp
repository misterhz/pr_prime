#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include "range.h"

int THREADS_NUM = 0;

#define SHOW_INFO

bool* sieve_tab;
int* primes;

bool* sieve_in_range;
int* primes_in_range;

int count = 0;
int count_in_range = 0;

int find_primes_sieve_dynamic(int from, int to) {
    from = from >= 2 ? from : 2;
    int sqrt_max = (int) sqrt(to);
 
#pragma omp parallel
    {
#pragma omp for schedule(dynamic)
        for(int i = 0; i < sqrt_max - 1; i++) {
            if(!sieve_tab[i]) continue;
            int num = i + 2;
            for(int j = i + num; j < sqrt_max; j += num) {
                sieve_tab[j] = false;
            }
        }

#pragma omp single
        for(int i = 0; i < sqrt_max; i++) {
            if(sieve_tab[i]) {
                primes[count++] = i + 2;
            }
        }

#pragma omp for schedule(dynamic)
        for(int i = 0; i < count; i++) {
            int num = primes[i];
            int start_from = from - from % num;
            if(start_from == from && from != num) {
                sieve_in_range[start_from - from] = false;
            }
            int current_num = 0;
            if(start_from == 0) {
                current_num = start_from + 2 * num; // cause if sqrt(max) > from then bug appears
            } else {
                current_num = start_from + num;
            }
            for(; current_num < to; current_num += num) {
                sieve_in_range[current_num - from] = false;
            }
        }

#pragma omp single
        for(int i = 0; i < to - from; i++) {
            if(sieve_in_range[i]) {
                primes_in_range[count_in_range++] = i + from;
            }
        }
    }
    return count_in_range;
}

int main(int argc, char** argv) {
   if(argc != 4) {
        printf("%s min max threads\n", argv[0]);
        return 1;
    }

    int min = atoi(argv[1]);
    int max = atoi(argv[2]);

    int sqrt_max = (int) sqrt(max);
    int threads_num = atoi(argv[3]);

    THREADS_NUM = threads_num;

    sieve_tab = (bool*) malloc(sizeof(bool) * sqrt_max);
    primes = (int*) malloc(sizeof(int) * sqrt_max / 2);

    sieve_in_range = (bool*) malloc(sizeof(bool) * (max - min));
    primes_in_range = (int*) malloc(sizeof(int) * (max - min));

    omp_set_num_threads(threads_num);

    memset(sieve_tab, true, sizeof(bool) * sqrt_max);
    memset(sieve_in_range, true, sizeof(bool) * (max - min));

    printf("%d %d\n", min, max);

    find_primes_sieve_dynamic(min, max);

    #ifdef SHOW_INFO
        printf("%d primes between %d and %d\n", count_in_range, min, max);
        printf("First up to 10 primes are:\n");
        for(int i = 0; i < 10 && i < count_in_range; i++) {
            printf("%d\n", primes_in_range[i]);
        }
    #endif

    // printf("\n%d\n", count_in_range);
}

