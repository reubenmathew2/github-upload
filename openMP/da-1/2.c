#include <stdio.h>
#include <omp.h>
#define n 100000
int main()
{
    double ang[4] = {0.0, 0.7, 0.867, 1.0};
    double seq[n], num = 0.0;
    float starttime, endtime, exectime;
    int i, k, c = 0;
    int omp_rank;
    float etime[20];
    int thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
    int thread_arr_size = 13;
    for (k = 0; k < thread_arr_size; k++)
    {
        omp_set_num_threads(thread[k]);
        starttime = omp_get_wtime();
#pragma omp parallel private(i) shared(seq, num)
        {
#pragma omp for
            for (i = 1; i < n; i++)
            {
                omp_rank = omp_get_thread_num();
                num = ((i * (i + 1.0)) / 2.0) * ang[(i - 1) % 4];
                seq[i] = num;
            }
        }
        endtime = omp_get_wtime();
        exectime = endtime - starttime;
        etime[k] = exectime;
        for (i = 0; i < n; i++)
            printf("\n%lf from thread %d", seq[i], omp_rank);
    }
    for (k = 0; k < thread_arr_size; k++)
        printf("\n Thread=%d\t Run Time=%f\n", thread[k], etime[k]);

    return 0;
}