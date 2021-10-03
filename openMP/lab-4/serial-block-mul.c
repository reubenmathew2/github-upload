#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int block[] = {5, 10, 50, 100};
int main()
{
    double st, et, tt[4];

    int L = 500, M = 500, N = 500;

    double matrix_x[500][500], matrix_y[500][500], matrix_z[500][500];

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
        st = omp_get_wtime();
        
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
                                r += matrix_x[i][k] * matrix_y[k][j];
                            }
                            matrix_z[i][j] = matrix_z[i][j] + r;
                        }
                    }
                }
            }
        
        et = omp_get_wtime();
        tt[bl] = et - st;
    }
    for (int i = 0; i < 4; i++)
    {
        printf("\n\nBlock Size: %d", block[i]);
        printf("\nrun time = %f\n", tt[i]);
    }
    return 0;
}

