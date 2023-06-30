#ifndef PGM_H
#define PGM_H

#include "../../structs/structs.h"

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

#endif // PGM_H
