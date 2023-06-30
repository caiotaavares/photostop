#ifndef IMAGEMANAGEMENT_H
#define IMAGEMANAGEMENT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../../structs/structs.h"

using namespace std;

/*
 * Ler imagens PPM
 */
Image read_ppm(const char* filename) {
    int row = 0, col = 0, numrows = 0, numcols = 0, maxval = 0;
    ifstream infile(filename);
    stringstream ss;
    string inputLine = "";

    Image image;
    Array** array = new Array*[numrows];

    // First line: version
    getline(infile, inputLine);
    cout << "Version: " << inputLine << endl;

    // Store version in Image structure
    string version = inputLine;
    cout << "Version: " << version << endl;

    // Second line: comment
    getline(infile, inputLine);
    cout << "Comment: " << inputLine << endl;

    // Store comment in Image structure
    string comment = inputLine;

    // Continue with a stringstream
    ss << infile.rdbuf();
    // Third line: size
    ss >> numcols >> numrows;
    cout << numcols << " columns and " << numrows << " rows" << endl;

    // Fourth line: maximum value
    ss >> maxval;
    cout << "Maximum value: " << maxval << endl;

    if (version == "P3") {
        Array** array = new Array*[numrows];
        for (row = 0; row < numrows; ++row) {
            array[row] = new Array[numcols];  // RGB channels
            for (col = 0; col < numcols; ++col) {
                ss >> array[row][col].R;
                ss >> array[row][col].G;
                ss >> array[row][col].B;
            }
        }

        image.version = version;
        image.comment = comment;
        image.array = array;
        image.numrows = numrows;
        image.numcols = numcols;
        image.maxval = maxval;
    }

    return image;
}

void savePPMP3(const char* filename, const Image& image) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error opening file " << filename << " for writing." << endl;
        return;
    }

    // Write PPM header
    outfile << "P3" << endl;
    outfile << "# Imagem Gerada por photoStop" << endl;
    outfile << image.numcols << " " << image.numrows << endl;
    outfile << "255" << endl;

    // Write the RGB pixels
    for (int row = 0; row < image.numrows; ++row) {
        for (int col = 0; col < image.numcols; ++col) {
            outfile << image.array[row][col].R << " ";
            outfile << image.array[row][col].G << " ";
            outfile << image.array[row][col].B << "  ";
        }
        //        outfile << endl;
    }

    outfile.close();
}

/*
 * Ler imagem Pgm
 */
ImagePgm read_pgm(const char* filename) {
    int row = 0, col = 0, numrows = 0, numcols = 0, maxval = 0;
    ifstream infile(filename);
    stringstream ss;
    string inputLine = "";

    // First line: version
    getline(infile, inputLine);
    cout << "Version: " << inputLine << endl;

    // Store version in Image structure
    string version = inputLine;
    cout << "Version: " << version << endl;

    // Second line: comment
    getline(infile, inputLine);
    cout << "Comment: " << inputLine << endl;

    // Store comment in Image structure
    string comment = inputLine;

    // Continue with a stringstream
    ss << infile.rdbuf();
    // Third line: size
    ss >> numcols >> numrows;
    cout << numcols << " columns and " << numrows << " rows" << endl;

    // Fourth line: maximum value
    ss >> maxval;
    cout << "Maximum value: " << maxval << endl;


    int **array = new int*[numrows];
    // Following lines : data
    for(row = 0; row < numrows; ++row) {
        array[row] = new int[numcols];
        for (col = 0; col < numcols; ++col) ss >> array[row][col];
    }


    ImagePgm image;
    image.version = version;
    image.comment = comment;
    image.array = array;
    image.numrows = numrows;
    image.numcols = numcols;
    image.maxval = maxval;

    return image;
}

/*
 * Salvar pgm
 */
void SavePGMP2(const char* filename, const ImagePgm& imagePgm) {
    int numcols = imagePgm.numcols;
    int numrows = imagePgm.numrows;
    int **array = imagePgm.array;
    ofstream outFile(filename);
    if (outFile) {
        outFile << "P2\n";
        outFile << "# CREATOR: Caio Tavares\n";
        outFile << numcols << " " << numrows << "\n";
        for (int row = 0; row < numrows; row++) {
            for (int col = 0; col < numcols; col++) {
                int pixel = array[row][col];
                if (row == 0 && col == 0) {
                    outFile << "255\n";
                } else {
                    outFile << pixel << "\n";
                }
            }
        }
        outFile.close();
    } else {
        cout << "Failed to open file for writing." << endl;
    }
}


#endif  // IMAGEMANAGEMENT_H
