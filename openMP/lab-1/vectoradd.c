#include <stdio.h>
#include <omp.h>
#define n 100000
#define m 100000
int main()
{
  double a[n], b[n], c[n];
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
#pragma omp parallel private(i) shared(a, b, c)
    {
#pragma omp for
      for (i = 0; i < n; i++)
      {
        omp_rank = omp_get_thread_num();
        a[i] = i * 10.236;
        b[i] = i * 152.123;
        for (int j = 0; j < m; j++)
          c[i] = a[i] + b[i];
        //printf("the value of a[%d] = %lf and b[%d] = %lf and result c[%d]
        // =%lf done by worker thread id =%d\n",i,a[i],i,b[i],i,c[i],omp_rank);
      }
    }
    endtime = omp_get_wtime();
    exectime = endtime - starttime;
    etime[k] = exectime;
  }
  for (k = 0; k < thread_arr_size; k++)
  {
    printf("\n Thread=%d\t etime=%f\n", thread[k], etime[k]);
  }
  return 0;
}