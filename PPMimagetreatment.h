#ifndef PPMIMAGETREATMENT_H
#define PPMIMAGETREATMENT_H

#include "structs.h"

int findMedianB(Array pixel[3][3]) {
    int values[9];
    int k = 0;

    // Extract the values from the 3x3 matrix and store them in an array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            values[k++] = pixel[i][j].B;
        }
    }

    // Sort the values in ascending order
    for (int i = 0; i < 9 - 1; i++) {
        for (int j = 0; j < 9 - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }

    // Return the median value, which is in the middle of the sorted array
    return values[5];
}

int findMedianG(Array pixel[3][3]) {
    int values[9];
    int k = 0;

    // Extract the values from the 3x3 matrix and store them in an array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            values[k++] = pixel[i][j].G;
        }
    }

    // Sort the values in ascending order
    for (int i = 0; i < 9 - 1; i++) {
        for (int j = 0; j < 9 - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }

    // Return the median value, which is in the middle of the sorted array
    return values[5];
}

int findMedianR(Array pixel[3][3]) {
    int values[9];
    int k = 0;

    // Extract the values from the 3x3 matrix and store them in an array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            values[k++] = pixel[i][j].R;
        }
    }

    // Sort the values in ascending order
    for (int i = 0; i < 9 - 1; i++) {
        for (int j = 0; j < 9 - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }

    // Return the median value, which is in the middle of the sorted array
    return values[5];
}

// Median filter
Image median_filter(Image image) {
    int filterSize = 3;
    Image filtered_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;
    filtered_image.array = (Array **)malloc(filtered_image.numrows * sizeof(Array *));
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (Array *)malloc(filtered_image.numcols * sizeof(Array));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            Array pixel[3][3] = {0};
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        pixel[i][j] = image.array[neighbor_row][neighbor_col];
                    }
                }
            }

            filtered_image.array[row][col].R = findMedianR(pixel);
            filtered_image.array[row][col].G = findMedianG(pixel);
            filtered_image.array[row][col].B = findMedianB(pixel);
        }
    }

    return filtered_image;
}

Array calculateAverage(Array pixel[3][3]) {
    Array averagePixel = {0, 0, 0};

    // Calculate the sum of RGB values for each pixel in the 3x3 matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            averagePixel.R += pixel[i][j].R;
            averagePixel.G += pixel[i][j].G;
            averagePixel.B += pixel[i][j].B;
        }
    }

    // Divide the sum by 9 to get the average RGB values
    averagePixel.R /= 9;
    averagePixel.G /= 9;
    averagePixel.B /= 9;

    return averagePixel;
}

// Average filter
Image average_filter(Image image) {
    int filterSize = 3;
    Image filtered_image;
    filtered_image.version = image.version;
    filtered_image.comment = image.comment;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.maxval = image.maxval;
    filtered_image.array = (Array **)malloc(filtered_image.numrows * sizeof(Array *));
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (Array *)malloc(filtered_image.numcols * sizeof(Array));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            Array pixel[3][3] = {0};
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        pixel[i][j] = image.array[neighbor_row][neighbor_col];
                    }
                }
            }

            filtered_image.array[row][col] = calculateAverage(pixel);
        }
    }

    return filtered_image;
}

#endif // PPMIMAGETREATMENT_H
