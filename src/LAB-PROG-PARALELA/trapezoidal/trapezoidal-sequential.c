#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float f(float x);
float calculaIntegral(float *limits, float deltaX, int nTrapezoidesPorRank);

int main(int argc, char **argv){
    clock_t begin = clock();

    int limitA = atoi(argv[1]);
    int limitB = atoi(argv[2]);
    int nTrapezoides = atoi(argv[3]);

    float deltaX = (float) (limitB - limitA)/nTrapezoides;

    int nTrapezoidesPorRank = nTrapezoides;

    float limits[2] = { limitA, limitB };

    float soma = calculaIntegral(limits, deltaX, nTrapezoidesPorRank);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("INTEGRAL: %f\nTIME: %f\n", soma, time_spent);
    
    return 0;
}

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