#include <mpi.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>

#define MAX 100

int main(int argc, char** argv) {
        double totalT; 
        int rank, size, i,stride, source,tag=1;
        float a[MAX];
        float acum = 0,total=0;
        MPI_Status stat;

        for (i = 1; i <= 100; i++)
        a[ i - 1 ] = i;

        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        stride = MAX/(size);
        int disp[stride],scounts[stride];
        float b[stride];

         for (i = 0; i < size; i++) {
                disp[i] = i*stride;
                scounts[i] = stride;
        }

        double tstart = MPI_Wtime();
        MPI_Scatterv(a,scounts,disp,MPI_FLOAT,b,stride,MPI_FLOAT,0,MPI_COMM_WORLD);
        double tfinish = MPI_Wtime();
        double TotalTime = tfinish - tstart;

        acum = 0.0;
        for (i = 0; i < stride; i++)
                acum += b[i];
        printf("Subtotal %lf en nodo %d tiempo %g\n", acum, rank,TotalTime);

        MPI_Reduce(&TotalTime,&totalT,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
        MPI_Reduce(&acum,&total,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
        if (rank == 0)
                printf("Total: %lf Tiempo: %g \n", total,totalT);
        MPI_Finalize();
}
