#include <stdio.h>
#include <omp.h>

#define n 50000
#define delay 50000

int main()
{
    double a[n],runtime[13];
    float startTime,endTime;
    int i,k,omp_rank;
    double sum, fsum;
    int threads[]={1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
    for(k=0;k<13;k++)
    {
        sum=0.0;
        omp_set_num_threads(threads[k]);
        startTime = omp_get_wtime();
        #pragma omp parallel private(i)
        {
            #pragma omp for 
            for(i=0;i<n;i++)
            {
                omp_rank = omp_get_thread_num();
                a[i] = (float) i * 5.52 ; 
                for(int j=0;j<delay;j++);
                sum += a[i];
            }
            #pragma omp critical(finalsum)
            fsum += sum;

        }
        endTime = omp_get_wtime();
        runtime[k] = endTime - startTime;
    }
    for(k=0;k<13;k++)
    printf("\n\nThread Count: %d      Run Time: %f",threads[k], runtime[k]);
    return 0;
}

/*
4.250000
2.143555
1.173828
0.790039
0.636719
0.696777
0.683105
0.643555
0.649414
0.659668
0.662598
0.646973
0.654297
*/