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
unsigned char *make_negative(unsigned char *pixels, int total) {
    unsigned char *negative = NULL;
    negative = malloc((size_t)total * sizeof(unsigned char));
    if (negative == NULL) {
        printf("no se pudo reservar memoria para el negativo.\n");
        return NULL;
    }
    for (int i = 0; i < total; i++) {
        *(negative + i) = (unsigned char)(255 - *(pixels + i));
    }
    return negative;
}
void write_pgm(const char *filename, unsigned char *pixels, int width, int height, int max_val) {
    FILE *file = NULL;
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo crear el archivo %s\n", filename);
        return;
    }
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "%d\n", max_val);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%d ", *(pixels + i * width + j));
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
//impresion de estadisticas
void print_stats(unsigned char *original, unsigned char *thresholded, int total) {
    int white = 0;
    int black = 0;
    long sum = 0;
    float average;
    for (int i = 0; i < total; i++) {
        sum += *(original + i);
        if (*(thresholded + i) == 255) {
            white++;
        } else if (*(thresholded + i) == 0) {
            black++;
        }
    }
    average = (float)sum / total;
    printf("Pixeles blancos: %d\n", white);
    printf("Pixeles negros: %d\n", black);
    printf("Promedio original: %.2f\n", average);
}

int main(void) {
    int width, height, max_val, threshold;
    int total;
    unsigned char *pixels = NULL;
    unsigned char *original = NULL;
    unsigned char *negative = NULL;
    pixels = read_pgm("input.pgm", &width, &height, &max_val);
    if (pixels == NULL) {
        return 1;
    }
    total = width * height;
    original = malloc((size_t)total * sizeof(unsigned char));
    if (original == NULL) {
        printf("Error: no se pudo reservar memoria para la copia original.\n");
        free(pixels);
        return 1;
    }
    for (int i = 0; i < total; i++) {
        *(original + i) = *(pixels + i);
    }
    printf("Ingrese el umbral T: ");
    if (scanf("%d", &threshold) != 1 || threshold < 0 || threshold > max_val) {
        printf("Umbral invalido.\n");
        free(original);
        free(pixels);
        return 1;
    }
    apply_threshold(pixels, total, threshold);
    negative = make_negative(pixels, total);
    if (negative == NULL) {
        free(original);
        free(pixels);
        return 1;
    }
    print_stats(original, pixels, total);
    write_pgm("output_threshold.pgm", pixels, width, height, 255);
    write_pgm("output_negative.pgm", negative, width, height, 255);
    printf("Archivos generados: output_threshold.pgm y output_negative.pgm\n");
    free(original);
    free(pixels);
    free(negative);
    return 0;
}