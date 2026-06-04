#include <stdio.h>
#include <stdlib.h>

unsigned char *read_pgm(const char *archivo, int *width, int *height, int *max_val) {
    FILE *file = NULL;
    unsigned char *pixels = NULL;
    char p, two;
    int total;
    int value;
    file = fopen(archivo, "r"); //apertura de archivo
    if (file == NULL) {
        printf("no se pudo abrir el archivo %s\n", archivo);
        return NULL;
    }
    if (fscanf(file, " %c%c", &p, &two) != 2) {
        printf("no se pudo leer el formato del archivo.\n");
        fclose(file);
        return NULL;
    }
    if (p != 'P' || two != '2') {
        printf("el archivo no esta en formato P2.\n");
        fclose(file);
        return NULL;
    }
    //leer alto y ancho
    if (fscanf(file, "%d %d", width, height) != 2) {
        printf("no se pudo leer el ancho y el alto.\n");
        fclose(file);
        return NULL;
    }

    //leer max de pixeles
    if (fscanf(file, "%d", max_val) != 1) {
        printf("no se pudo leer el valor maximo.\n");
        fclose(file);
        return NULL;
    }

    //validacion de valores del archivo
    if (*width <= 0 || *height <= 0 || *max_val <= 0 || *max_val > 255) {
        printf("Error: datos invalidos en el encabezado.\n");
        fclose(file);
        return NULL;
    }
    total = (*width) * (*height);

    pixels = malloc((size_t)total * sizeof(unsigned char));
    if (pixels == NULL) {
        printf("no se pudo reservar memoria para los pixeles.\n");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < total; i++) {
        if (fscanf(file, "%d", &value) != 1) {
            printf("Error: no se pudo leer un pixel.\n");
            free(pixels);
            fclose(file);
            return NULL;
        }
        if (value < 0 || value > *max_val) {
            printf("ixel fuera de rango.\n");
            free(pixels);
            fclose(file);
            return NULL;
        }
        *(pixels + i) = (unsigned char)value;
    }
    fclose(file);
    return pixels;
}

void apply_threshold(unsigned char *pixels, int total, int threshold) {
    for (int i = 0; i < total; i++) {
        if (*(pixels + i) >= threshold) {
            *(pixels + i) = 255;
        } 
        else {
            *(pixels + i) = 0;
        }
    }
}