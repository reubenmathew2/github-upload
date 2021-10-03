#include <stdio.h>
#include <omp.h>

#define n 50000
#define delay 50000

int main()
{
    double a[n], b[n], c[n], runtime[13];
    float startTime,endTime;
    int i,k,omp_rank;
    double dot;
    int threads[]={1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
    for(k=0;k<13;k++)
    {
        dot=0.0;
        omp_set_num_threads(threads[k]);
        startTime = omp_get_wtime();
        #pragma omp parallel private(i)
        {
            #pragma omp for reduction (+:dot)
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
        }
        endTime = omp_get_wtime();
        runtime[k] = endTime - startTime;
    }
    for(k=0;k<13;k++)
    printf("\n\nThread Count: %d      Run Time: %f",threads[k], runtime[k]);
    return 0;
}

/*
Thread Count: 1      Run Time: 7.463867

Thread Count: 2      Run Time: 3.953125

Thread Count: 4      Run Time: 2.590088

Thread Count: 6      Run Time: 2.109863

Thread Count: 8      Run Time: 2.073242

Thread Count: 10      Run Time: 2.187256

Thread Count: 12      Run Time: 2.781982

Thread Count: 16      Run Time: 2.784180

Thread Count: 20      Run Time: 2.803711

Thread Count: 32      Run Time: 2.784180

Thread Count: 64      Run Time: 2.795654

Thread Count: 128      Run Time: 2.791016

Thread Count: 150      Run Time: 2.824951
*/