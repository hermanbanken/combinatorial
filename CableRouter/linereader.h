#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include "structures.h"

class LineReader {
private:
    ifstream in;

public:
    LineReader(const string filename);

    vector<coordinate> getLine();
};
