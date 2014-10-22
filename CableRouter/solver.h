#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include <vector>
#include <tuple>
#include "structures.h"

#ifndef ___cablerouter_solver___
#define ___cablerouter_solver___

using namespace std;

namespace Solvers {

    class Solver {
    public:
        virtual void preprocess(Grid *grid, double &time) {
            time = 0;
        }
        virtual void solve(Grid *grid, vector<coordinate> &line, double &time) = 0;
    };

}

#endif /* defined(___cablerouter_solver___) */