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
        virtual void solve(vector<vector<cell>> &grid, vector<tuple<unsigned long,unsigned long>> &line) = 0;
    };

}

#endif /* defined(___cablerouter_solver___) */