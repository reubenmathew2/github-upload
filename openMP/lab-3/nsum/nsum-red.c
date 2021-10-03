#include <stdio.h>
#include <omp.h>

#define n 50000
#define delay 50000

int main()
{
    double a[n],runtime[13];
    float startTime,endTime;
    int i,k,omp_rank;
    double sum;
    int threads[]={1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
    for(k=0;k<13;k++)
    {
        sum=0.0;
        omp_set_num_threads(threads[k]);
        startTime = omp_get_wtime();
        #pragma omp parallel private(i)
        {
            #pragma omp for reduction (+:sum)
            for(i=0;i<n;i++)
            {
                omp_rank = omp_get_thread_num();
                a[i] = (float) i * 5.52 ; 
                    for(int j=0;j<delay;j++)
                        a[i] += 1.23;
                sum += a[i];
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
Thread Count: 1      Run Time: 6.741821

Thread Count: 2      Run Time: 3.389893

Thread Count: 4      Run Time: 1.996948

Thread Count: 6      Run Time: 1.459961

Thread Count: 8      Run Time: 1.221313

Thread Count: 10      Run Time: 1.310669

Thread Count: 12      Run Time: 1.304443

Thread Count: 16      Run Time: 1.272217

Thread Count: 20      Run Time: 1.328979

Thread Count: 32      Run Time: 1.675537

Thread Count: 64      Run Time: 1.725830

Thread Count: 128      Run Time: 1.742554

Thread Count: 150      Run Time: 1.755737%     
*/