#ifndef PGM_H
#define PGM_H

#include "../../structs/structs.h"
#include <algorithm>
#include <math.h>

/*
 * Helpers
 */
int findMedian(int **pixel, int filterHeight) {
    int values[filterHeight * filterHeight];
    int k = 0;

    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterHeight; j++) {
            values[k++] = pixel[i][j];
        }
    }

    for (int i = 0; i < filterHeight * filterHeight - 1; i++) {
        for (int j = 0; j < filterHeight * filterHeight - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }

    return values[(filterHeight * filterHeight) / 2];
}

/*
 * Median Filter
 */
ImagePgm median_filter_pgm(const ImagePgm& imagepgm, int filterHeight) {
    int filterSize = 3;
    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    int** pixel = new int*[filterHeight];
    for (int i = 0; i < filterHeight; i++) {
        pixel[i] = new int[filterHeight];
    }

    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new int[filtered_image.numcols];
    }

    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            for (int i = 0; i < filterHeight; i++) {
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < imagepgm.numcols &&
                        neighbor_row >= 0 && neighbor_row < imagepgm.numrows) {
                        pixel[i][j] = imagepgm.array[neighbor_row][neighbor_col];
                    } else {
                        pixel[i][j] = 0; // or any other appropriate value for out-of-bounds pixels
                    }
                }
            }
            filtered_image.array[row][col] = findMedian(pixel, filterHeight);
        }
    }

    for (int i = 0; i < filterHeight; i++) {
        delete[] pixel[i];
    }
    delete[] pixel;

    return filtered_image;
}

/*
 * Filtro da média
 */
ImagePgm average_filter_pgm(const ImagePgm& imagepgm, int filterHeight) {
    int filterSize = 3;
    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    int** pixel = new int*[filterHeight];
    for (int i = 0; i < filterHeight; i++) {
        pixel[i] = new int[filterHeight];
    }

    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new int[filtered_image.numcols];
    }

    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            int sum = 0;
            int count = 0;
            for (int i = 0; i < filterHeight; i++) {
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < imagepgm.numcols &&
                        neighbor_row >= 0 && neighbor_row < imagepgm.numrows) {
                        pixel[i][j] = imagepgm.array[neighbor_row][neighbor_col];
                        sum += pixel[i][j];
                        count++;
                    }
                }
            }
            filtered_image.array[row][col] = sum / count;
        }
    }

    for (int i = 0; i < filterHeight; i++) {
        delete[] pixel[i];
    }
    delete[] pixel;

    return filtered_image;
}

/*
 * Laplaciano de máscara diagonal
 */
ImagePgm laplacePgm(ImagePgm imagepgm) {
    int filterSize = 3;
    int laplacianFilter[3][3] = {
         {0 ,-1, 0},
         {-1, 4, -1},
         {0 ,-1, 0}
    };

    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            int sum = 0;
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_row >= 0 && neighbor_row < imagepgm.numrows &&
                        neighbor_col >= 0 && neighbor_col < imagepgm.numcols) {
                        sum += imagepgm.array[neighbor_row][neighbor_col] * laplacianFilter[i][j];
                    }
                }
            }
            filtered_image.array[row][col] = clamp(sum, 0, 255);
        }
    }

    return filtered_image;
}

/*
 * Laplaciano de vizinhança 8
 */
ImagePgm laplacePgm_8(ImagePgm imagepgm) {
    int filterSize = 3;
    int laplacianFilter[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            int sum = 0;
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_row >= 0 && neighbor_row < imagepgm.numrows &&
                        neighbor_col >= 0 && neighbor_col < imagepgm.numcols) {
                        sum += imagepgm.array[neighbor_row][neighbor_col] * laplacianFilter[i][j];
                    }
                }
            }
            filtered_image.array[row][col] = clamp(sum, 0, 255);
        }
    }

    return filtered_image;
}

/*
 *  Efeiro de bluring (borramento)
 */
ImagePgm blurringPgm(ImagePgm imagepgm, double filterHeight) {

    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            int sum = 0;
            int count = 0;
            for (int i = -filterHeight/2; i <= filterHeight/2; i++) {
                for (int j = -filterHeight/2; j <= filterHeight/2; j++) {
                    int neighbor_row = row + i;
                    int neighbor_col = col + j;
                    if (neighbor_row >= 0 && neighbor_row < imagepgm.numrows &&
                        neighbor_col >= 0 && neighbor_col < imagepgm.numcols) {
                        sum += imagepgm.array[neighbor_row][neighbor_col];
                        count++;
                    }
                }
            }
            filtered_image.array[row][col] = sum / count;
        }
    }

    return filtered_image;
}

/*
 * Filtro High Boost
 */
ImagePgm highBoost_filter_pgm(ImagePgm imagepgm, double boostFactor) {
    int filterSize = 3;

    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    // Cria uma imagem borrada
    ImagePgm blurred_image = blurringPgm(imagepgm, boostFactor);

    // Cria uma máscara subtraindo a imagem borrada da original
    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            int mask_value = imagepgm.array[row][col] - blurred_image.array[row][col];
            int filtered_value = (int)(imagepgm.array[row][col] + boostFactor * mask_value);
            filtered_image.array[row][col] = clamp(filtered_value, 0, 255);
        }
    }

    return filtered_image;
}

/*
 *  Equalização local de histograma
 */
ImagePgm local_histogram_equalization_pgm(ImagePgm imagepgm) {
    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];

    int hist[256] = {0};
    int cdf[256] = {0};
    float alpha = 255.0 / 9.0;  // Valor de alpha para o espaço 3x3

    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            // Zerar o histograma
            for (int i = 0; i < 256; i++) {
                hist[i] = 0;
            }

            // Calcular o histograma para o espaço 3x3
            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < imagepgm.numrows && j >= 0 && j < imagepgm.numcols) {
                        hist[imagepgm.array[i][j]]++;
                    }
                }
            }

            // Calcular a função de distribuição cumulativa
            int sum = 0;
            for (int i = 0; i < 256; i++) {
                sum += hist[i];
                cdf[i] = round(alpha * sum);
            }

            // Aplicar a equalização para o pixel atual
            imagepgm.array[row][col] = cdf[imagepgm.array[row][col]];
        }
    }

    return filtered_image;
}

/*
 * Equalização global
 */
ImagePgm histogram_equalization_pgm(ImagePgm imagepgm) {
    ImagePgm filtered_image;
    filtered_image.version = imagepgm.version;
    filtered_image.comment = imagepgm.comment;
    filtered_image.numrows = imagepgm.numrows;
    filtered_image.numcols = imagepgm.numcols;
    filtered_image.maxval = imagepgm.maxval;
    filtered_image.array = new int*[filtered_image.numrows];
    for (int row = 0; row < imagepgm.numrows; ++row) {
        filtered_image.array[row] = new int[filtered_image.numcols];
    }

    int hist[256] = {0};
    int cdf[256] = {0};
    int pixels = imagepgm.numrows * imagepgm.numcols;
    float alpha = 255.0 / pixels;

    // Calculate the histogram
    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            hist[imagepgm.array[row][col]]++;
        }
    }

    // Calculate the cumulative distribution function
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += hist[i];
        cdf[i] = round(alpha * sum);
    }

    // Apply the equalization to each pixel
    for (int row = 0; row < imagepgm.numrows; row++) {
        for (int col = 0; col < imagepgm.numcols; col++) {
            filtered_image.array[row][col] = cdf[imagepgm.array[row][col]];
        }
    }

    return filtered_image;
}

/*
 * Negativo
 */
ImagePgm negative(ImagePgm imagepgm) {

    // darken the image
    for(int row = 0; row < imagepgm.numrows; ++row) {
        for(int col = 0; col < imagepgm.numcols; ++col) {
            imagepgm.array[row][col] = 255 - imagepgm.array[row][col];
        }
    }
    imagepgm.array[0][0] = 255;
    return imagepgm;
}

/*
 * Girar +90
 */
ImagePgm turn_plus_90(ImagePgm imagepgm) {
    ImagePgm sup_image;
    sup_image.numrows = imagepgm.numcols; // Swap the rows and columns for the new image
    sup_image.numcols = imagepgm.numrows;

    // Allocate memory for the new image
    sup_image.array = new int*[sup_image.numrows];
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = new int[sup_image.numrows];
    }

    // Populate the new image
    for (int row = 0; row < sup_image.numcols; ++row) {
        for (int col = 0; col < sup_image.numrows; ++col) {
            sup_image.array[col][row] = imagepgm.array[imagepgm.numrows-1-row][col];
        }
    }

    return sup_image;
}

/*
 * Girar -90
 */
ImagePgm turn_minus_90(ImagePgm imagepgm) {
    ImagePgm sup_image;
    sup_image.numrows = imagepgm.numcols;
    sup_image.numcols = imagepgm.numrows;

    sup_image.array = new int*[sup_image.numrows];
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = new int[sup_image.numrows];
    }

    // Populate the new image
    for (int row = 0; row < sup_image.numcols; ++row) {
        for (int col = 0; col < sup_image.numrows; ++col) {
            sup_image.array[col][row] = imagepgm.array[row][imagepgm.numcols-1-col];
        }
    }

    return sup_image;
}

/*
 * Girar 180
 */
ImagePgm turn_plus_180(ImagePgm imagepgm) {
    ImagePgm sup_image;
    sup_image.numrows = imagepgm.numrows;
    sup_image.numcols = imagepgm.numcols;

    sup_image.array = new int*[sup_image.numrows];
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = new int[sup_image.numrows];
    }

    for (int row = 0; row < sup_image.numrows; ++row) {
        for (int col = 0; col < sup_image.numcols; ++col) {
            sup_image.array[row][col] = imagepgm.array[imagepgm.numrows - 1 - row][imagepgm.numcols - 1 - col];
        }
    }

    return sup_image;
}

/*
 * Binarização
 */
ImagePgm binarizing(ImagePgm image, int k) {
    for (int row = 0; row != image.numrows; row++) {
        for (int col = 0; col != image.numcols; col++) {
            if (image.array[row][col] <= k) image.array[row][col] = 0;
            else image.array[row][col] = 255;
        }
    }

    return image;
}

#endif // PGM_H
