compile_all : naive_static naive_dynamic sieve_domain_dynamic sieve_functional_dynamic
	rm ./*.o

naive_dynamic : naive_dynamic.o
	g++ -fopenmp -o naive_dynamic naive_dynamic.o -lm -O3
naive_static : naive_static.o
	g++ -fopenmp -o naive_static naive_static.o -lm -O3
sieve_domain_dynamic : sieve_domain_dynamic.o 
	g++ -fopenmp -o sieve_domain_dynamic sieve_domain_dynamic.o range.o -lm -O3
sieve_functional_dynamic : sieve_functional_dynamic.o 
	g++ -fopenmp -o sieve_functional_dynamic sieve_functional_dynamic.o range.o -lm -O3
sieve_functional_static : sieve_functional_static.o 
	g++ -fopenmp -o sieve_functional_static sieve_functional_static.o range.o -lm -O3

naive_dynamic.o : naive_dynamic.cpp
	g++ -fopenmp -c naive_dynamic.cpp -O3
naive_static.o : naive_static.cpp
	g++ -fopenmp -c naive_static.cpp -O3
sieve_domain_dynamic.o : sieve_domain_dynamic.cpp range.cpp range.h
	g++ -fopenmp -c sieve_domain_dynamic.cpp range.h range.cpp
sieve_functional_dynamic.o : sieve_functional_dynamic.cpp range.cpp range.h
	g++ -fopenmp -c sieve_functional_dynamic.cpp range.h range.cpp
sieve_functional_static.o : sieve_functional_static.cpp range.cpp range.h
	g++ -fopenmp -c sieve_functional_static.cpp range.h range.cpp
