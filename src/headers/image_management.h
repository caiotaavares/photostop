#ifndef IMAGEMANAGEMENT_H
#define IMAGEMANAGEMENT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../../structs/structs.h"

using namespace std;

Image read_image(const char* filename) {
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

    Array** array = new Array*[numrows];
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

        Image image;
        image.version = version;
        image.comment = comment;
        image.array = array;
        image.numrows = numrows;
        image.numcols = numcols;
        image.maxval = maxval;
    }

    if (version == "P2") {
        int **array = new int*[numrows];
        // Following lines : data
        for(row = 0; row < numrows; ++row) {
            array[row] = new int[numcols];
            for (col = 0; col < numcols; ++col) ss >> array[row][col];
        }

        Image image;
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

#endif  // IMAGEMANAGEMENT_H
