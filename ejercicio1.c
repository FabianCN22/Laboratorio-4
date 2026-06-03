#include <stdio.h>
#include <stdlib.h>

//funcion que recorre la matriz para encontyr secuencias de 1
void findLargestLine(int **matrix, int size, int *result) {
    int total = size * size;      //total de elementos
    int currentLine = 0;          //Cantidad actual de 1 consecutivos
    int largestLine = 0;          //Mayor cantidad de 1 consecutivos

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
