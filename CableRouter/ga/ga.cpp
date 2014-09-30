#include "ga.h"
#include <tuple>
#include <iostream>
#include <vector>
#include "../solver.h"
#include "cmaes.h"

using namespace std;
using namespace libcmaes;
using namespace Solvers;

FitFunc fsphere = [](const double *x, const int N)
{
    double val = 0.0;
    for (int i=0;i<N;i++)
        val += x[i]*x[i];
    return val;
};

void GA::solve(vector<vector<cell>> &grid, vector<tuple<unsigned long, unsigned long>> &line) {
}