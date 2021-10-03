#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char argv[])
{
    int omp_rank, omp_threads;

    #pragma omp parallel private(omp_rank)
    {
        omp_rank = omp_get_thread_num();
        omp_threads = omp_get_num_threads();
        
        printf("Hello World! from thread number %d of threads %d\n", omp_rank, omp_threads);
    }
}

// gcc -fopenmp hello-world.c 
// export OMP_NUM_THREADS=1
// ./a.out