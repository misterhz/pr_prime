#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include "range.h"

#define MAX 100'000
#define MIN 2
#define SIZE MAX - MIN
#define THREADS_NUM 7

#define SHOW_INFO

bool sieve_tab[(int) sqrt(MAX)] = {true};
int primes[(int) (sqrt(MAX) / 2)];

bool sieve_in_range[MAX - MIN];
int primes_in_range[MAX - MIN];

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

#pragma omp barrier

#pragma omp single
        for(int i = 0; i < sqrt_max; i++) {
            if(sieve_tab[i]) {
                primes[count++] = i + 2;
//                printf("%d\n", i + 2);
                // printf("%d\n", primes[count - 1]);
            }
        }

        int id = omp_get_thread_num();

        int local_range_start = get_range_start(THREADS_NUM, id, from, to);
        int local_range_end = get_range_end(THREADS_NUM, id, from, to);

//        printf("%d: gonna check from %d to %d\n", id, local_range_start, local_range_end);

        // #pragma omp for
        for(int i = 0; i < count; i++) {
            int num = primes[i];
            int start_from = local_range_start - local_range_start % num;
            if(start_from == local_range_start && local_range_start != num) {
                sieve_in_range[local_range_start - from] = false;
            }
            int current_num = 0;
            if(start_from == 0) {
                current_num = start_from + 2 * num; // cause if sqrt(max) > from then bug appears
            } else {
                current_num = start_from + num;
            }
//            printf("%d: %d\n", id, current_num);
            for(; current_num < local_range_end; current_num += num) {
                sieve_in_range[current_num - from] = false;
//                printf("%d: %d isn\'t prime\n", id, current_num);
            }
        }
        #pragma omp barrier

#pragma omp single
        for(int i = 0; i < to - from; i++) {
            if(sieve_in_range[i]) {
                primes_in_range[count_in_range++] = i + from;
                // printf("%d\n", primes_in_range[count_in_range - 1]);
            }
        }
    }

    return 0;
}

int main() {
    omp_set_num_threads(THREADS_NUM);
    memset(sieve_tab, true, sizeof(sieve_tab));
    memset(sieve_in_range, true, sizeof(sieve_in_range));
    find_primes_sieve_dynamic(MIN, MAX);

#ifdef SHOW_INFO
    printf("%d primes between %d and %d\n", count_in_range, MIN, MAX);
    printf("First up to 10 primes are:\n");
    for(int i = 0; i < 10 && i < count_in_range; i++) {
        printf("%d\n", primes_in_range[i]);
    }
#endif

    // printf("\n%d\n", count_in_range);
}

