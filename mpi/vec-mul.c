#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define LEN 1000
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main(int argc, char *argv[])
{
    int numtasks, taskid, numworkers, source, dest, mtype, segment, aveseg, extra, offset, i, j, k, rc;
    long double a[LEN], b[LEN], c[LEN];
    double start, end, runtime;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    start = MPI_Wtime();

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
        for (i = 0; i < LEN; i++)
                a[i] = i * 1.2;
        for (j = 0; j < LEN; j++)
                b[j] = j * 3.4;

        aveseg = LEN / numworkers;
        extra = LEN % numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++)
        {
            segment = (dest <= extra) ? aveseg + 1 : aveseg;

            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&segment, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset], segment, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&b[offset], segment, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + segment;
        }

        //receive from worker:

        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&segment, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset], segment, MPI_LONG_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
        }

        end = MPI_Wtime();

        printf("\nResultant Vector:\n");

        for (i = 0; i < LEN; i++)
            printf("%6.2Lf   ", c[i]);
        
        printf("\nDone.\n");
        
        printf("\nRun Time: %lf\n", end - start);
    }
    //Worker task:

    if (taskid > MASTER)
    {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&segment, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, segment, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, segment, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        char pro_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name(pro_name,&name_len);
        printf("\nWorking in Processor %s\n",pro_name);
        
        //mat addition
        
        for (i = 0; i < segment; i++)
            c[i] = a[i] * b[i];
            

        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&segment, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, segment, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}