#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NR 20
#define NC 20
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main(int argc, char *argv[])
{
    int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j, k, rc;
    long double a[NR][NC], b[NC][NC], c[NR][NC];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks < 2)
    {
        printf("Need atleast two MPI tasks. Quiting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }
    numworkers = numtasks - 1;

    //master task:

    if (taskid == MASTER)
    {
        for (i = 0; i < NR; i++)
            for (j = 0; j < NC; j++)
                a[i][j] = i + j;
        for (i = 0; i < NC; i++)
            for (j = 0; j < NC; j++)
                b[i][j] = i * j;

        averow = NR / numworkers;
        extra = NR % numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++)
        {
            rows = (dest <= extra) ? averow + 1 : averow;

            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows * NC, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&b[offset][0], rows * NC, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }

        //receive from worker:

        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows * NC, MPI_LONG_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
        }

        printf("\nResultant Matrix:\n");
        for (i = 0; i < NR; i++)
        {
            printf("\n");
            for (j = 0; j < NC; j++)
                printf("%6.2Lf   ", c[i][j]);
        }
        printf("\nDone.\n");
    }
    //Worker task:

    if (taskid > MASTER)
    {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows * NC, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, rows * NC, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        //mat addition
        for (k = 0; k < NC; k++)
            for (i = 0; i < rows; i++)
            {
                c[i][k] = a[i][k] + b[i][k];
            }

        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, rows * NC, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}