#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

const int N = 100000;

int main()
{
  int i, k, omp_rank;
  float start, end, run;

  float rtime[20];
  int thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
  int thread_arr_size = 13;
  double res[N];

  for (k = 0; k < thread_arr_size; k++)
  {
    omp_set_num_threads(thread[k]);
    start = omp_get_wtime();
#pragma omp parallel private(i) shared(res)
    {
#pragma omp for
      for (i = 0; i < N; i++)
      {
        omp_rank = omp_get_thread_num();
        res[i] = 8 * pow(0.7, i);
      }
    }
    end = omp_get_wtime();
    run = end - start;
    rtime[k] = run;
  }
  for (int i = 0; i < N; i++)
    cout << res[i] << " ";
  for (k = 0; k < thread_arr_size; k++)
    cout << "\nThread=" << thread[k] << " RunTime=" << rtime[k];
  return 0;
}