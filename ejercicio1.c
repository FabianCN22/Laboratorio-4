#include <stdio.h>
#include <stdlib.h>

//funcion que recorre la matriz para encontyr secuencias de 1
void findLargestLine(int **matrix, int size, int *result) {
    int total = size * size;   //total de elementos
    int currentLine = 0;       //antidad actual de 1 consecutivos
    int largestLine = 0;        //Mayor cantidad de 1 consecutivos

    int *p = *(matrix + 0);
    for (int i = 0; i < total; i++) {
        if (*(p + i) == 1) {
            currentLine++;
            if (currentLine > largestLine) {
                largestLine = currentLine;
            }
        } else {
            currentLine = 0;
        }
    }
    *result = largestLine;
}

//funcion para asignar memoria a la matriz
void allocateMatrix(int ***matrix, int size) {
    int *data = NULL;     //puntero con valor conocido
    *matrix = malloc((size_t)size * sizeof(int *));
    if (*matrix == NULL) {
        printf("Error: no se pudo reservar memoria para las filas.\n");
        exit(1);
    }
    data = malloc((size_t)size * (size_t)size * sizeof(int));
    if (data == NULL) {
        printf("Error: no se pudo reservar memoria para los datos.\n");
        free(*matrix);
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        *(*matrix + i) = data + i * size;
    }
}

//genera la matriz con numeros del 0 al 1 alea
void fillMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            *(*(matrix + i) + j) = rand() % 2;
        }
    }
}