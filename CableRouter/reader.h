#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include "structures.h"

class Reader {
private:
    ifstream in;

public:
    Reader(const string filename);

    int next(point *row);
};