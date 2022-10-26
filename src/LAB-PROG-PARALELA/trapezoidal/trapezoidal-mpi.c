#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

float f(float x){
    return x * x;
}

float calculaIntegral(float *limits, float deltaX, int nTrapezoidesPorRank){
    float soma = (f(limits[0]) + f(limits[1]))/2.0;
    float x = limits[0];

    for(int i = 0; i < nTrapezoidesPorRank; i++){
        x += deltaX;
        soma += f(x);
    }

    return soma * deltaX;
}

int main(int argc, char **argv){
    clock_t begin = clock();

    int size;
    int rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int limitA = atoi(argv[1]);
    int limitB = atoi(argv[2]);
    int nTrapezoides = atoi(argv[3]);

    float deltaX = (float) (limitB - limitA)/nTrapezoides;

    int nTrapezoidesPorRank = nTrapezoides/size;

    float a = limitA + (rank * nTrapezoidesPorRank * deltaX);
    float b = (rank == size - 1) ? limitB : a + (nTrapezoidesPorRank * deltaX);

    if(nTrapezoides % size > 0){
        if(rank < (nTrapezoides % size)){
            nTrapezoidesPorRank++;
            a = (limitA + (rank * nTrapezoidesPorRank * deltaX));
            b = (a + nTrapezoidesPorRank * deltaX);
        } else {
            a = (limitA + rank * nTrapezoidesPorRank * deltaX) + ((nTrapezoides % size) * deltaX);
            b += (rank == size - 1) ? 0 : ((nTrapezoides % size) * deltaX);
        }
    }
    
    float limits[2] = { a, b };

    float integralParcial = calculaIntegral(limits, deltaX, nTrapezoidesPorRank);
    float integral;

    MPI_Reduce(&integralParcial, &integral, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    if(rank == 0) printf("INTEGRAL: %f\nTIME: %f\n", integral, time_spent);

    MPI_Finalize();

    return 0;
}