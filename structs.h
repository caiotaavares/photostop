#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using namespace std;

typedef struct array {
    int R;
    int G;
    int B;
} Array;

typedef struct image {
    string version;
    string comment;
    Array **array;
    int numrows;
    int numcols;
    int maxval;
} Image;

#endif // STRUCTS_H
