#include "ga.h"
#include <tuple>
#include <iostream>
#include <vector>
#include "../solver.h"
#include <stdexcept>
#include "cmaes.h"
#include <math.h>
#include "../structures.h"

using namespace std;
using namespace libcmaes;
using namespace Solvers;

Solvers::GA::GA(int points) {
    this->points = points;
}

void GA::solve(Grid* grid, vector<tuple<unsigned long, unsigned long>> &line) {
    if(line.empty())
        throw invalid_argument("line was empty: it should contain at least 2 points");

    // Grid
    this->grid = grid;
    Projection id = Projection::identity();

    // Move last point to correct position
    this->start = line.front();
    this->end = line.back();
    line.erase (line.end());
    line.reserve((unsigned long) this->points+2);
    line.insert(line.end(), this->end);

    FitFunc fitness = [grid,this,id](const double *x, const int N)->double {
        double val = 0.0;
        float angle = grid->angle(get<0>(this->start), get<1>(this->start), x[0], x[1], id);
        // From windmill
        val += grid->cost(get<0>(this->start), get<1>(this->start), x[0], x[1], id);
        for (int i = 0; i+3 < N; i+=4){
            // Distance
            val += grid->cost(x[i], x[i+1], x[i+2], x[i+3], id);
            // Angle
            float new_angle = grid->angle(x[i], x[i+1], x[i+2], x[i+3], id);
            val += grid->cost(angle - new_angle);
            angle = new_angle;
        }
        // To windmill
        float new_angle = grid->angle(x[N-2], x[N-1], get<0>(this->end), get<1>(this->end), id);
        val += grid->cost(x[N-2], x[N-1], get<0>(this->end), get<1>(this->end), id);
        val += grid->cost(angle - new_angle);
        return val;
    };

    // problem dimensions: this->points
    vector<double> x0(this->points*2,10.0);
    double sigma = 0.1;
    //int lambda = 100; // offsprings at each generation.
    CMAParameters<> cmaparams(this->points*2,&x0.front(),sigma);
    //cmaparams.set_algo(BIPOP_CMAES);
    CMASolutions cmasols = cmaes<>(fitness,cmaparams);
    cout << "best solution: " << cmasols << endl;
    cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds" << endl;
    int o = cmasols.run_status();
    cout << "Result: " << o << endl;
}