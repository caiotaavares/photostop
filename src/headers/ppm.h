#ifndef PPMIMAGETREATMENT_H
#define PPMIMAGETREATMENT_H

#include "../../structs/structs.h"
#include <algorithm>
#include <cmath>

using namespace std;

// Um algoritmo mais rápido pode melhorar a velocidade
// em filtros muito grandes
//
int findMedian(int *values, int filterSize) {
    int median = filterSize / 2;

    // Quickselect algorithm
    int low = 0;
    int high = filterSize - 1;
    while (low < high) {
        int pivotIndex = low + (high - low) / 2;
        int pivotValue = values[pivotIndex];
        int i = low - 1;
        int j = high + 1;
        while (true) {
            do {
                i++;
            } while (values[i] < pivotValue);
            do {
                j--;
            } while (values[j] > pivotValue);
            if (i >= j) {
                break;
            }
            int temp = values[i];
            values[i] = values[j];
            values[j] = temp;
        }
        if (j < median) {
            low = j + 1;
        } else if (j > median) {
            high = j - 1;
        } else {
            break;
        }
    }

    return values[median];
}

//
// FILTRO DA MEDIANA
//
Image median_filter(const Image& image, int filterHeight) {
    int filterSize = filterHeight * filterHeight;

    Image filtered_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;
    filtered_image.array = new Array*[filtered_image.numrows];
    Array* filterMatrix = new Array[filterSize];

    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; col++) {
            int k = 0;

            // Extrai os valores da matriz
            for (int i = 0; i < filterHeight; i++) {
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        filterMatrix[k++] = image.array[neighbor_row][neighbor_col];
                    }
                }
            }

            // Encontra o valor da mediana pra cada canal (R,G,B)
            int* valuesR = new int[filterSize];
            int* valuesG = new int[filterSize];
            int* valuesB = new int[filterSize];
            for (int i = 0; i < filterSize; i++) {
                valuesR[i] = filterMatrix[i].R;
                valuesG[i] = filterMatrix[i].G;
                valuesB[i] = filterMatrix[i].B;
            }

            filtered_image.array[row][col].R = findMedian(valuesR, filterSize);
            filtered_image.array[row][col].G = findMedian(valuesG, filterSize);
            filtered_image.array[row][col].B = findMedian(valuesB, filterSize);

            delete[] valuesR;
            delete[] valuesG;
            delete[] valuesB;
        }
    }

    // Desaloca a memória
    delete[] filterMatrix;

    return filtered_image;
}

//
// FILTRO DA MEDIA
//
Array calculateAverage(Array **pixel, int filterHeight) {
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;

    // Calculate the sum of each channel
    for (int i = 0; i < filterHeight; i++) {
        for (int j = 0; j < filterHeight; j++) {
            sumR += pixel[i][j].R;
            sumG += pixel[i][j].G;
            sumB += pixel[i][j].B;
        }
    }

    // Calculate the average value for each channel
    int numPixels = filterHeight * filterHeight;
    Array averagePixel;
    averagePixel.R = sumR / numPixels;
    averagePixel.G = sumG / numPixels;
    averagePixel.B = sumB / numPixels;

    return averagePixel;
}

Image average_filter(const Image& image, int filterHeight) {
    Image filtered_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;

    filtered_image.array = new Array*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; col++) {
            Array **pixel = new Array*[filterHeight];
            for (int i = 0; i < filterHeight; i++) {
                pixel[i] = new Array[filterHeight];
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        pixel[i][j] = image.array[neighbor_row][neighbor_col];
                    } else {
                        // Handle out-of-bounds pixels by copying the current pixel
                        pixel[i][j] = image.array[row][col];
                    }
                }
            }

            filtered_image.array[row][col] = calculateAverage(pixel, filterHeight);

            // Deallocate memory for the filter matrix
            for (int i = 0; i < filterHeight; i++) {
                delete[] pixel[i];
            }
            delete[] pixel;
        }
    }

    return filtered_image;
}

//
// ISOLAR CANAIS
//
Image r(const Image& image, char channel) {
    Image filtered_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;

    filtered_image.array = new Array*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; ++col) {
            if (channel == 'R') {
                filtered_image.array[row][col].R = image.array[row][col].R;
                filtered_image.array[row][col].G = 0;
                filtered_image.array[row][col].B = 0;
            } else if (channel == 'G') {
                filtered_image.array[row][col].R = 0;
                filtered_image.array[row][col].G = image.array[row][col].G;
                filtered_image.array[row][col].B = 0;
            } else if (channel == 'B' ){
                filtered_image.array[row][col].R = 0;
                filtered_image.array[row][col].G = 0;
                filtered_image.array[row][col].B = image.array[row][col].B;
            }

        }
    }

    return filtered_image;
}

//
// LAPLACIANO DIAGONAL
//
Image laplace(const Image& image) {
    Image filtered_image;
    int filterHeight = 3;
    int laplacianFilter[3][3] {
        {0 ,-1, 0},
        {-1, 4, -1},
        {0 ,-1, 0}
    };
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;

    filtered_image.array = new Array*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; col++) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            for (int i = 0; i < filterHeight; i++) {
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        sumR += image.array[neighbor_row][neighbor_col].R * laplacianFilter[i][j];
                        sumG += image.array[neighbor_row][neighbor_col].G * laplacianFilter[i][j];
                        sumB += image.array[neighbor_row][neighbor_col].B * laplacianFilter[i][j];
                        //                        printf("%d\n", sumR);
                        //                        printf("%d\n", sumG);
                        //                        printf("%d\n", sumB);
                        //                        printf("\n");
                    } else {
                        filtered_image.array[row][col] = image.array[row][col];
                    }
                }
            }

            filtered_image.array[row][col].R = clamp(sumR, 0, 255) / 9;
            filtered_image.array[row][col].G = clamp(sumG, 0, 255) / 9;
            filtered_image.array[row][col].B = clamp(sumB, 0, 255) / 9;

            filtered_image.array[row][col].R += image.array[row][col].R;
            filtered_image.array[row][col].G += image.array[row][col].G;
            filtered_image.array[row][col].B += image.array[row][col].B;
            //            printf("%d\n", filtered_image.array[row][col].R);
            //            printf("%d\n", filtered_image.array[row][col].G);
            //            printf("%d\n", filtered_image.array[row][col].B);
            //            printf("\n");
        }
    }

    return filtered_image;
}

//
// LAPLACIANO VIZINHANÇA 8
//
Image laplace_8(const Image& image) {
    Image filtered_image;
    int filterHeight = 3;
    int laplacianFilter[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;

    filtered_image.array = new Array*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; col++) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            for (int i = 0; i < filterHeight; i++) {
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        sumR += image.array[neighbor_row][neighbor_col].R * laplacianFilter[i][j];
                        sumG += image.array[neighbor_row][neighbor_col].G * laplacianFilter[i][j];
                        sumB += image.array[neighbor_row][neighbor_col].B * laplacianFilter[i][j];
                        //                        printf("%d\n", sumR);
                        //                        printf("%d\n", sumG);
                        //                        printf("%d\n", sumB);
                        //                        printf("\n");
                    } else {
                        filtered_image.array[row][col] = image.array[row][col];
                    }
                }
            }

            filtered_image.array[row][col].R = clamp(sumR, 0, 255) / 9;
            filtered_image.array[row][col].G = clamp(sumG, 0, 255) / 9;
            filtered_image.array[row][col].B = clamp(sumB, 0, 255) / 9;

            filtered_image.array[row][col].R += image.array[row][col].R;
            filtered_image.array[row][col].G += image.array[row][col].G;
            filtered_image.array[row][col].B += image.array[row][col].B;
            //            printf("%d\n", filtered_image.array[row][col].R);
            //            printf("%d\n", filtered_image.array[row][col].G);
            //            printf("%d\n", filtered_image.array[row][col].B);
            //            printf("\n");
        }
    }

    return filtered_image;
}

//
// Filtro High Boost
//
Image blurring(const Image& image, double filterHeight) {
    Image filtered_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;

    filtered_image.array = new Array*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; col++) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            int count = 0;
            for (int i = 0; i < filterHeight; i++) {
                for (int j = 0; j < filterHeight; j++) {
                    int neighbor_row = row - filterHeight / 2 + i;
                    int neighbor_col = col - filterHeight / 2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        sumR += image.array[neighbor_row][neighbor_col].R;
                        sumG += image.array[neighbor_row][neighbor_col].G;
                        sumB += image.array[neighbor_row][neighbor_col].B;
                        count++;
                    }
                }
            }

            filtered_image.array[row][col].R = sumR / count;
            filtered_image.array[row][col].G = sumG / count;
            filtered_image.array[row][col].B = sumB / count;
        }
    }

    return filtered_image;
}

Image high_boost(const Image& image, double boostFactor) {
    Image filtered_image, blurred_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;

    blurred_image = blurring(image, 9);

    filtered_image.array = new Array*[filtered_image.numrows];
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = new Array[filtered_image.numcols];
        for (int col = 0; col < filtered_image.numcols; ++col) {
            int mask_valueR = (int)(image.array[row][col].R - blurred_image.array[row][col].R);
            int mask_valueG = (int)(image.array[row][col].G - blurred_image.array[row][col].G);
            int mask_valueB = (int)(image.array[row][col].B - blurred_image.array[row][col].B);

            int filtered_valueR = (int)(image.array[row][col].R + boostFactor * mask_valueR);
            int filtered_valueG = (int)(image.array[row][col].G + boostFactor * mask_valueG);
            int filtered_valueB = (int)(image.array[row][col].B + boostFactor * mask_valueB);

            filtered_image.array[row][col].R = clamp(filtered_valueR, 0, 255);
            filtered_image.array[row][col].G = clamp(filtered_valueG, 0, 255);
            filtered_image.array[row][col].B = clamp(filtered_valueB, 0, 255);
        }
    }

    return filtered_image;
}

//
// Equalização global
//
Image histogram_equalization(Image image) {
    Image down_image;
    down_image.numrows = image.numrows;
    down_image.numcols = image.numcols;
    down_image.array = new Array*[down_image.numrows];
    for (int row = 0; row < down_image.numrows; ++row) {
        down_image.array[row] = new Array[down_image.numcols];
    }

    int histR[256] = {0};
    int histG[256] = {0};
    int histB[256] = {0};
    int cdfR[256] = {0};
    int cdfG[256] = {0};
    int cdfB[256] = {0};
    int pixels = image.numrows * image.numcols;
    float alpha = 255.0 / pixels;

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            histR[image.array[row][col].R]++;
            histG[image.array[row][col].G]++;
            histB[image.array[row][col].B]++;
        }
    }

    int sumR = 0;
    int sumG = 0;
    int sumB = 0;
    for (int i = 0; i < 256; i++) {
        sumR += histR[i];
        sumG += histG[i];
        sumB += histB[i];
        cdfR[i] = round(alpha * sumR);
        cdfG[i] = round(alpha * sumG);
        cdfB[i] = round(alpha * sumB);
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            int pixelR = cdfR[image.array[row][col].R];
            int pixelG = cdfG[image.array[row][col].G];
            int pixelB = cdfB[image.array[row][col].B];
            down_image.array[row][col].R = pixelR;
            down_image.array[row][col].G = pixelG;
            down_image.array[row][col].B = pixelB;
        }
    }

    return down_image;
}
#endif // PPMIMAGETREATMENT_H
