#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using namespace std;

/*
 * Structs para PPM
 */
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

/*
 * Structs para PGM
 */
typedef struct {
    string version;
    string comment;
    int **array;
    int numrows;
    int numcols;
} ImagePgm;

#endif // STRUCTS_H
