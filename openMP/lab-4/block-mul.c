#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};

int block[] = {60, 70, 80, 90};
int main()
{
    double st, et, tt[4][13];

    int omp_rank, omp_threads, L = 500, M = 500, N = 500;

    double matrix_x[500][500], matrix_y[500][500], matrix_z[500][500];

    #pragma omp for collapse(2)
     for (int i = 0; i < 500; i++)
     {
         for (int j = 0; j < 500; j++)
         {
            matrix_x[i][j] = (i * 5.123);
            matrix_y[i][j] = (j * 3.123);
            matrix_z[i][j] = 0;
         }
     } 
    for(int bl = 0; bl < 4; bl++)
    {
        for (int k = 0; k < 13; k++)
        {
            omp_set_num_threads(thread[k]);
            st = omp_get_wtime();
            #pragma omp parallel private(omp_rank)
            {  
                omp_rank = omp_get_thread_num();
                omp_threads = omp_get_num_threads();

                #pragma omp for
                for(int jj = 0; jj < N; jj = jj + block[bl])
                {
                    for(int kk = 0; kk < N; kk = kk + block[bl])
                    {
                        for(int i = 0; i < N; i++)
                        {
                            for(int j = jj; j < MIN(jj + block[bl], N); j++)
                            {
                                double r = 0;
                                for(int k = kk; k < MIN(kk + block[bl], N); k++)
                                {
                                    for (int z = 0; z < 100; z++);
                                    r += matrix_x[i][k] * matrix_y[k][j];
                                }
                                matrix_z[i][j] = matrix_z[i][j] + r;
                            }
                        }
                    }
                }
            }
            et = omp_get_wtime();
            tt[bl][k] = et - st;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        printf("\n\nBlock Size: %d", block[i]);
        for(int j = 0; j < 13; j++)
            printf("\nthreads =%d time = %f\n", thread[j], tt[i][j]);
    }
  return 0;
}

/*

Block Size: 5
23.486433
12.013435
6.157926
4.935832
4.772059
4.881417
4.811815
4.766318
4.736443
4.787231
4.676044
4.656539
4.701207


Block Size: 10
23.224251
11.856178
6.262981
5.844545
4.971684
4.868007
5.120404
5.037133
4.791715
4.767400
4.766442
4.780197
4.790802


Block Size: 50
23.009945
11.746762
7.319166
6.397539
6.303351
4.973113
7.167268
6.164324
6.834658
6.793960
6.989790
6.967308
7.408360

Block Size: 60
22.953027
13.906575
8.407262
5.897808
5.854035
4.229139
4.680710
4.699177
4.680541
4.784448
4.718529
4.632021
4.634609


Block Size: 70
23.151415
13.101539
7.272827
7.838773
5.014907
5.089790
5.045320
5.110050
5.169086
5.173267
5.125316
5.150665
5.150174


Block Size: 80
23.005722
14.805051
7.587125
8.810107
5.354247
5.702993
5.686340
6.122624
5.701152
5.698317
5.734176
5.826881
5.803925


Block Size: 90
22.946544
12.376577
9.161485
6.223140
6.249338
6.282035
6.262360
6.316504
6.228981
6.290286
6.260243
6.265207
6.254129

Block Size: 100
25.309478
14.050808
9.816426
6.463053
6.853781
6.790966
6.851798
6.842600
6.855366
6.823517
6.795096
6.825074
6.853850
*/