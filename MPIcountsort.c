#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mpi.h>
#include <string.h>  

int main(int argc, char *argv[]){
        long filesize;
        char* filename = (char*) malloc ( 50 * sizeof(char));
        FILE *fptr; MPI_Init (&argc, &argv); int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank); int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int* originalarray; int* workerarray;
        if (rank == 0) {
                scanf("%s", filename);
                fptr = fopen(filename, "rb");
                fseek(fptr, 0, SEEK_END);
                filesize = ftell(fptr);
                fseek(fptr, 0, SEEK_SET);
                originalarray = malloc(filesize);
                fread(originalarray, filesize, 1, fptr);
                fclose(fptr);}
        MPI_Bcast(&filesize, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        workerarray = malloc(filesize);
        if (rank==0) {
                memcpy(workerarray, originalarray, filesize);
                free(originalarray);}
        int countarray[1000] = {0};
        int final[1000] = {0};
        int numofthreads = 4;
        int len = filesize/numofthreads;
        MPI_Bcast(workerarray, len, MPI_INT, 0, MPI_COMM_WORLD);
        int chunk = len / size;
        int start = rank * chunk;
        int end = start + chunk;
        long value;
        for(int i = start; i < end; i++){
            value = workerarray[i];
            countarray[value] += 1;}
        MPI_Reduce(countarray, final, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if(rank == 0){
            fptr = fopen(filename, "wb");
            for(int i = 0; i < 1000; i++){
                for (int j = 0; j < final[i]; j++) {
                    fwrite(&i, sizeof(int), 1, fptr); } }
        } MPI_Finalize();
    return 0; }