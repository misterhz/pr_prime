#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

// #define SHOW_INFO

int* primes;
int primes_count = 0;

int find_primes_naive_static(int from, int to) {
    if(to < 2 || to <= from) return 0;
    
    if(from == 2){
        ++primes_count;
        primes[0] = 2;
    }
    if(from % 2 == 0)
        from++;

    #pragma omp parallel for schedule(static)
    for(int i = from; i < to; i += 2) {
        bool is_prime = true;
        int last_num = sqrt((double) i);
        for(int j = 3; j <= last_num; j += 2) {
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

int main(int argc, char** argv) {
    if(argc != 4) {
        printf("%s min max threads\n", argv[0]);
        return 1;
    }

    int min = atoi(argv[1]);
    int max = atoi(argv[2]);
    int tab_size = (int) (max - min) / 2;
    int threads_num = atoi(argv[3]);

    primes = (int*) malloc(sizeof(int) * tab_size);

    omp_set_num_threads(threads_num);
    printf("%d\n", find_primes_naive_static(min, max));

    #ifdef SHOW_INFO
        printf("%d primes between %d and %d\n", primes_count, MIN, MAX);
        printf("First up to 10 primes are:\n");
        for(int i = 0; i < 10 && i < primes_count; i++) {
            printf("%d\n", primes[i]);
        }
    #endif

    // for(int i = 0; i < primes_count; i++) {
    //     printf("%lld\n", primes[i]);
    // }
}