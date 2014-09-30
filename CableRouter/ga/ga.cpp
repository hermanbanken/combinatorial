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
    int dim = 10; // problem dimensions.
    vector<double> x0(dim,10.0);
    double sigma = 0.1;
    //int lambda = 100; // offsprings at each generation.
    CMAParameters<> cmaparams(dim,&x0.front(),sigma);
    //cmaparams.set_algo(BIPOP_CMAES);
    CMASolutions cmasols = cmaes<>(fsphere,cmaparams);
    cout << "best solution: " << cmasols << endl;
    cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds" << endl;
    int o = cmasols.run_status();
    cout << "Result: " << o << endl;
}