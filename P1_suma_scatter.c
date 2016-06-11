#define MAX 100

int main(int argc, char** argv) {
        int rank, size, i,stride, source,tag=1,total=0;
        float a[MAX];
        float acum = 0;
        MPI_Status stat;

        for (i = 1; i <= 100; i++)
        a[ i - 1 ] = i;
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        stride = MAX/(size)+1;
        int disp[stride],scounts[stride];
        float b[stride];

         for (i = 0; i < size; i++) {
                disp[i] = i*stride;
                scounts[i] = stride;
        }

        MPI_Scatterv(a,scounts,disp,MPI_FLOAT,b,stride,MPI_FLOAT,0,MPI_COMM_WORLD);
        acum = 0.0;
        for (i = 0; i < stride; i++)
                acum += b[i];
        printf("Subtotal %lf en nodo %d\n", acum, rank);

        MPI_Reduce(&acum,b,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
        if (rank == 0) printf("TOTAL: %lf\n",b[0]);
        MPI_Finalize();
}

