#include <omp.h>
#include <stdio.h>
#define M 500
#define N 500
int thread[] = {1, 2, 4, 6, 8, 12, 16, 20, 32, 64};
int thread_size = 10;
int main()
{
  double m1[M][N], m2[M][N], ans[M][N];
  float st, et, t_execution[10];
  int i, j;
  for (int k = 0; k < thread_size; k++)
  {
    omp_set_num_threads(thread[k]);
    st = omp_get_wtime();
#pragma omp parallel private(i, j) shared(m1, m2, ans)
    {
#pragma omp for collapse(2)
      for (i = 0; i < M; i++)
      {
        for (j = 0; j < N; j++)
        {
          m1[i][j] = (i * 56.124);
          m2[i][j] = (i * 34.7758);
          for (int l = 0; l < 100000; l++)
            ans[i][j] = m1[i][j] + m2[i][j];
        }
      }
    }
    et = omp_get_wtime();
    t_execution[k] = et - st;
  }
  for (int i = 0; i < 10; i++)
    printf("\nthreads =%d time = %f\n", thread[i], t_execution[i]);
  return 0;
}