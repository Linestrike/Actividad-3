#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "image_processing.h"

const char *FILENAMES[30] = {
    "1.bmp", "2.bmp", "3.bmp", "4.bmp", "5.bmp", "6.bmp", "7.bmp", "8.bmp", "9.bmp", "10.bmp",
    "11.bmp", "12.bmp", "13.bmp", "14.bmp", "15.bmp", "16.bmp", "17.bmp", "18.bmp", "19.bmp", "20.bmp",
    "21.bmp", "22.bmp", "23.bmp", "24.bmp", "25.bmp", "26.bmp", "27.bmp", "28.bmp", "29.bmp", "30.bmp"
};

int main() {
    omp_set_num_threads(800);

    const char *ruta_entrada = "Imagenes/";
    const char *ruta_salida1 = "ByN/";
    const char *ruta_salida2 = "Rotacion/";
    const char *ruta_salida3 = "Espejo/";
    const char *ruta_salida4 = "Blur/";

    double start = omp_get_wtime();

    // Procesa imágenes en paralelo
    #pragma omp parallel for
    for (int i = 0; i < 30; i++) {
        char input_path[MAX_PATH], output_gris[MAX_PATH], output_invV[MAX_PATH], output_invH[MAX_PATH], output_blur[MAX_PATH];
        const char *filename = FILENAMES[i];

        build_path(input_path, ruta_entrada, filename);
        build_path(output_gris, ruta_salida1, filename);
        build_path(output_invV, ruta_salida2, filename);
        build_path(output_invH, ruta_salida3, filename);
        build_path(output_blur, ruta_salida4, filename);

        printf("[HILO %d] -> Procesando '%s'\n", omp_get_thread_num(), filename);

        bmp_gris(input_path, output_gris);             // Convierte a escala de grises
        bmp_invertirV(input_path, output_invV);        // Espejeo
        bmp_invertirH(input_path, output_invH);        // Rotacion
        bmp_blur(input_path, output_blur, 57);         // Desenfoque
    }

    double end = omp_get_wtime();
    printf("\nTiempo total: %.4f segundos\n", end - start);
    return 0;
}
