#include <omp.h>
#include <stdio.h>
#define M 500
int thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
int main()
{
  double ar1[M][M], ar2[M][M], ans[M][M];
  float st, et, tt[13];
  int i, j, l;
#pragma omp for collapse(2)
  for (int i = 0; i < M; i++)
    for (int j = 0; j < M; j++)
    {
      ar1[i][j] = (i * 5.123);
      ar2[i][j] = (j * 3.123);
      ans[i][j] = 0;
    }
  for (int k = 0; k < 13; k++)
  {
    omp_set_num_threads(thread[k]);
    st = omp_get_wtime();
#pragma omp parallel private(i, j, l) shared(ar1, ar2, ans)
    {
#pragma omp for
      for (i = 0; i < M; i++)
      {
        for (j = 0; j < M; j++)
        {
          double r = 0;
          for (l = 0; l < M; l++)
          {
            //for (int z = 0; z < 500; z++)
              
              r += ar1[i][l] * ar2[l][j];
              
          }
          ans[i][j] += r;
          //printf("\n%f %d %d",ans[i][j],i,j);
        }
      }
    }
    et = omp_get_wtime();
    tt[k] = et - st;
  }
  for (int i = 0; i < 13; i++)
    printf("\nthreads =%d time = %f\n", thread[i], tt[i]);
  return 0;
}