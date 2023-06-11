#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using namespace std;

typedef struct image {
    string version;
    string comment;
    int ***array;
    int numrows;
    int numcols;
    int maxval;
} Image;

#endif // STRUCTS_H
