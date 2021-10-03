#include <stdio.h>
#include <omp.h>
#include <math.h>
//#include <bits/stdc++.h>
#define n 100000

int main()
{
    double r = 0.707;
    double seq[n];
    double p, num;
    float starttime, endtime, exectime;
    int i, k;
    int omp_rank;
    float etime[20];
    int thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
    int thread_arr_size = 13;
    for (k = 0; k < thread_arr_size; k++)
    {
        omp_set_num_threads(thread[k]);
        starttime = omp_get_wtime();
#pragma omp parallel private(i) shared(seq)
        {
#pragma omp for
            for (i = 0; i < n; i++)
            {
                omp_rank = omp_get_thread_num();
                seq[i] = 8 * pow(0.707, i);  
            }
        }
        endtime = omp_get_wtime();
        exectime = endtime - starttime;
        etime[k] = exectime;
        for (i = 0; i < 5; i++)
            printf("\n%lf from thread %d", seq[i], omp_rank);
    }
    for (k = 0; k < thread_arr_size; k++)
        printf("\n Thread=%d\t Run Time=%f\n", thread[k], etime[k]);

    return 0;
}