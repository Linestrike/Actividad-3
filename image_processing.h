#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define HEADER_SIZE 54
#define MAX_PATH 256

static inline void build_path(char *buffer, const char *prefix, const char *filename) {
    strcpy(buffer, prefix);
    strcat(buffer, filename);
}

// Escala de grises

static inline void bmp_gris(const char *input_name, const char *output_name) {
    FILE *image = fopen(input_name, "rb");
    FILE *outputImage = fopen(output_name, "wb");

    if (image == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo abrir el archivo de entrada: %s\n", input_name); }
        return;
    }
    if (outputImage == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo crear/abrir el archivo de salida: %s\n", output_name); }
        fclose(image);
        return;
    }

    unsigned char header[HEADER_SIZE];
    long ancho = 0, alto = 0;
    
    // Cabecera y dimensiones
    for (int i = 0; i < HEADER_SIZE; i++) { 
        header[i] = fgetc(image); 
        fputc(header[i], outputImage);
    }
    ancho = (long)header[18] + ((long)header[19] << 8) + ((long)header[20] << 16) + ((long)header[21] << 24);
    alto  = (long)header[22] + ((long)header[23] << 8) + ((long)header[24] << 16) + ((long)header[25] << 24);

    unsigned char* arr_gray = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char b, g, r, pixel;
    for (long i = 0; i < ancho * alto; i++) {
        b = fgetc(image); g = fgetc(image); r = fgetc(image);
        pixel = 0.21*r + 0.72*g + 0.07*b;
        arr_gray[i] = pixel;
    }
    for (long i = ancho * alto; i > 0; i--) {
        fputc(arr_gray[(ancho * alto) - i], outputImage);
        fputc(arr_gray[(ancho * alto) - i], outputImage);
        fputc(arr_gray[(ancho * alto) - i], outputImage);
    }

    free(arr_gray);
    fclose(image); fclose(outputImage);
    printf("Imagen '%s' procesada y guardada como '%s'.\n", input_name, output_name);
}

// Espejo

static inline void bmp_invertirV(const char *input_name, const char *output_name) {
    FILE *image = fopen(input_name, "rb");
    FILE *outputImage = fopen(output_name, "wb");

    if (image == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo abrir el archivo de entrada: %s\n", input_name); }
        return;
    }
    if (outputImage == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo crear/abrir el archivo de salida: %s\n", output_name); }
        fclose(image);
        return;
    }

    unsigned char header[HEADER_SIZE];
    long ancho = 0, alto = 0, padding = 0;
    for (int i = 0; i < HEADER_SIZE; i++) { 
        header[i] = fgetc(image); 
        fputc(header[i], outputImage);
    }
    ancho = (long)header[18] + ((long)header[19] << 8) + ((long)header[20] << 16) + ((long)header[21] << 24);
    alto  = (long)header[22] + ((long)header[23] << 8) + ((long)header[24] << 16) + ((long)header[25] << 24);
    padding = (4 - (ancho * 3) % 4) % 4;

    unsigned char* pad = (unsigned char*)malloc(alto * padding * sizeof(unsigned char));
    unsigned char* arr_b = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_g = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_r = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));

    for (long fila = 0; fila < alto; fila++) {
        for (long col = 0; col < ancho; col++) {
            long idx = fila * ancho + col;
            arr_b[idx] = fgetc(image);
            arr_g[idx] = fgetc(image);
            arr_r[idx] = fgetc(image);
        }
        for (long p = 0; p < padding; p++) pad[fila * padding + p] = fgetc(image);
    }
    for (long fila = alto - 1; fila >= 0; fila--) {
        for (long col = 0; col < ancho; col++) {
            long idx = fila * ancho + col;
            fputc(arr_b[idx], outputImage);
            fputc(arr_g[idx], outputImage);
            fputc(arr_r[idx], outputImage);
        }
        for (long p = 0; p < padding; p++) fputc(pad[fila * padding + p], outputImage);
    }

    free(arr_b); free(arr_g); free(arr_r); free(pad);
    fclose(image); fclose(outputImage);
    printf("Imagen '%s' procesada y guardada como '%s'.\n", input_name, output_name);
}

// Rotacion

static inline void bmp_invertirH(const char *input_name, const char *output_name) {
    FILE *image = fopen(input_name, "rb");
    FILE *outputImage = fopen(output_name, "wb");

    if (image == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo abrir el archivo de entrada: %s\n", input_name); }
        return;
    }
    if (outputImage == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo crear/abrir el archivo de salida: %s\n", output_name); }
        fclose(image);
        return;
    }

    unsigned char header[HEADER_SIZE];
    long ancho = 0, alto = 0, padding = 0;
    for (int i = 0; i < HEADER_SIZE; i++) { 
        header[i] = fgetc(image); 
        fputc(header[i], outputImage);
    }
    ancho = (long)header[18] + ((long)header[19] << 8) + ((long)header[20] << 16) + ((long)header[21] << 24);
    alto  = (long)header[22] + ((long)header[23] << 8) + ((long)header[24] << 16) + ((long)header[25] << 24);
    padding = (4 - (ancho * 3) % 4) % 4;

    unsigned char* pad = (unsigned char*)malloc(alto * padding * sizeof(unsigned char));
    unsigned char* arr_b = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_g = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    unsigned char* arr_r = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));

    for (long fila = 0; fila < alto; fila++) {
        for (long col = 0; col < ancho; col++) {
            long idx = fila * ancho + col;
            arr_b[idx] = fgetc(image);
            arr_g[idx] = fgetc(image);
            arr_r[idx] = fgetc(image);
        }
        for (long p = 0; p < padding; p++) pad[fila * padding + p] = fgetc(image);
    }
    for (long fila = 0; fila < alto; fila++) {
        for (long col = ancho - 1; col >= 0; col--) {
            long idx = fila * ancho + col;
            fputc(arr_b[idx], outputImage);
            fputc(arr_g[idx], outputImage);
            fputc(arr_r[idx], outputImage);
        }
        for (long p = padding - 1; p >= 0 ; p--) fputc(pad[fila * padding + p], outputImage);
    }

    free(arr_b); free(arr_g); free(arr_r); free(pad);
    fclose(image); fclose(outputImage);
    printf("Imagen '%s' procesada y guardada como '%s'.\n", input_name, output_name);
}

// Blur

static inline void bmp_blur(const char *input_name, const char *output_name, const int mm) {
    FILE *image = fopen(input_name, "rb");
    FILE *outputImage = fopen(output_name, "wb");

    if (image == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo abrir el archivo de entrada: %s\n", input_name); }
        return;
    }
    if (outputImage == NULL) {
        #pragma omp critical
        { printf("ERROR: No se pudo crear/abrir el archivo de salida: %s\n", output_name); }
        fclose(image);
        return;
    }

    unsigned char xx[HEADER_SIZE];
    long ancho = 0, alto = 0;
    for (int i = 0; i < HEADER_SIZE; i++) { 
        xx[i] = fgetc(image); 
        fputc(xx[i], outputImage);
    }
    ancho = (long)xx[18] + ((long)xx[19] << 8) + ((long)xx[20] << 16) + ((long)xx[21] << 24);
    alto  = (long)xx[22] + ((long)xx[23] << 8) + ((long)xx[24] << 16) + ((long)xx[25] << 24);
    int widthWithPadding = (ancho * 3 + 3) & -4;
    long totalDataSize = (long)widthWithPadding * alto;

    unsigned char *pixelData = (unsigned char *)malloc(totalDataSize * sizeof(unsigned char));
    for (long i = 0; i < totalDataSize; i++) pixelData[i] = fgetc(image);

    // Progreso
    float progreso;
    printf("\n");
    int kernelRadius = (mm - 1) / 2;

    #pragma omp parallel
    {
        int contador1 = 0;
        #pragma omp for
        for (int y = 0; y < alto; y++) {
            contador1++;
            progreso = ((float)contador1 / (float)alto) * 100.0;

            if (y % 100 == 0 || y == alto - 1) {
                printf("[BLUR %s] Avance %0.2f%%\n", input_name, progreso);
            }

            for (long x = 0; x < ancho; x++) {
                unsigned int rSum = 0, gSum = 0, bSum = 0, pond = 0;
                for (int ky = -kernelRadius; ky <= kernelRadius; ky++) {
                    for (int kx = -kernelRadius; kx <= kernelRadius; kx++) {
                        int i = y + ky, j = x + kx;
                        if (i < 0 || i >= alto || j < 0 || j >= ancho) continue;
                        long index = (i * widthWithPadding) + (j * 3);
                        bSum += pixelData[index];
                        gSum += pixelData[index + 1];
                        rSum += pixelData[index + 2];
                        pond++;
                    }
                }
                long index = (y * widthWithPadding) + (x * 3);
                pixelData[index] = bSum / pond;
                pixelData[index + 1] = gSum / pond;
                pixelData[index + 2] = rSum / pond;
            }
        }
    }
    printf("[BLUR %s] Fin procesamiento\n", input_name);

    for (long i = 0; i < totalDataSize; i++) fputc(pixelData[i], outputImage);
    fclose(image); fclose(outputImage);
    free(pixelData);
}

#endif // IMAGE_PROCESSING_H
