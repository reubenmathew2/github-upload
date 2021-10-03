#include <stdio.h>
#include <omp.h>
#define n 100000
#define m 10000
int main()
{
  //double a[n], b[n], c[n];
  int seq[n], st = 2, num = 0;
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
#pragma omp parallel private(i) shared(seq, num)
    {
    #pragma omp for
      for (i = 0; i < n; i++)
      {
        seq[i] = 0;
        omp_rank = omp_get_thread_num();
        num = st + (3 * i);
        //for (int j = 0; j < m; j++); //delay
        seq[i] = num;
      }
    }
    endtime = omp_get_wtime();
    exectime = endtime - starttime;
    etime[k] = exectime;
    for (i = 0; i < n; i++)
        printf("%d ", seq[i]);
  }
  for (k = 0; k < thread_arr_size; k++)
  {
    printf("\n Thread=%d\t Run Time=%f\n", thread[k], etime[k]);
  }
  return 0;
}