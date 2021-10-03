#include <stdio.h>
#include <omp.h>

#define n 50000
#define delay 50000

int main()
{
    double a[n], b[n], c[n], runtime;
    float startTime,endTime;
    int i,k,omp_rank;
    double dot;
    dot=0.0;
    startTime = omp_get_wtime();
    for(i=0;i<n;i++)
    {
        a[i] = (float) i * 5.52;
        b[i] = (float) i * 3.23;
        c[i] = 0.0;
            for(int j=0;j<delay;j++)
                c[i] += a[i] * b[i];
        dot += c[i];
    }
    endTime = omp_get_wtime();
    runtime = endTime - startTime;
    printf("\n\nRun Time: %f", runtime);
    return 0;
}
