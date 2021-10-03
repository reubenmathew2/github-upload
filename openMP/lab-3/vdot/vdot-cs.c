#include <stdio.h>
#include <omp.h>

#define n 50000
#define delay 50000

int main()
{
    double a[n], b[n], c[n], runtime[13];
    float startTime,endTime;
    int i,k,omp_rank;
    double dot, fdot;
    int threads[]={1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
    for(k=0;k<13;k++)
    {
        dot=0.0;
        omp_set_num_threads(threads[k]);
        startTime = omp_get_wtime();
        #pragma omp parallel private(i)
        {
            #pragma omp for
            for(i=0;i<n;i++)
            {
                omp_rank = omp_get_thread_num();
                a[i] = (float) i * 5.52;
                b[i] = (float) i * 3.23;
                c[i] = 0.0;
                    for(int j=0;j<delay;j++)
                        c[i] += a[i] * b[i];
                dot += c[i];
            }
            #pragma omp critical(finaldot)
            fdot += dot;
        }
        endTime = omp_get_wtime();
        runtime[k] = endTime - startTime;
    }
    for(k=0;k<13;k++)
    printf("\n\nThread Count: %d      Run Time: %f",threads[k], runtime[k]);
    return 0;
}

/*
7.541016
4.117188
2.522949
2.166992
2.028320
2.218262
2.114746
2.333984
2.797852
2.791016
2.802246
2.804688
2.785645
*/