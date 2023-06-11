#ifndef IMAGEMANAGEMENT_H
#define IMAGEMANAGEMENT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "structs.h"

using namespace std;

Image read_image(const char *filename) {
    int row = 0, col = 0, numrows = 0, numcols = 0, maxval = 0;
    ifstream infile(filename);
    stringstream ss;
    string inputLine = "";

    // First line: version
    getline(infile, inputLine);
    if (inputLine.compare("P3") != 0) {
        cerr << "Version error" << endl;
    } else {
        cout << "Version: " << inputLine << endl;
    }

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

    // Create a 3D array for RGB channels
    int ***array = new int**[numrows];
    for (row = 0; row < numrows; ++row) {
        array[row] = new int*[numcols];
        for (col = 0; col < numcols; ++col) {
            array[row][col] = new int[3]; // RGB channels
            for (int i = 0; i < 3; ++i) {
                ss >> array[row][col][i];
            }
        }
    }

    Image image;
    image.version = version;
    image.comment = comment;
    image.array = array;
    image.numrows = numrows;
    image.numcols = numcols;
    image.maxval = maxval;

    return image;
}
#endif // IMAGEMANAGEMENT_H
