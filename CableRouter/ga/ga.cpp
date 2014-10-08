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

Solvers::GA::GA(unsigned int points) {
    this->points = points;
}

vector<coord> GA::straightLine(coord start, coord end, unsigned long points) {
    vector<coord> line(points);
    unsigned long x = 0;
    unsigned long y = 0;

    for(unsigned int i = 0; i + 1 < points; i++){
        x = (unsigned  long) round(start.first  + i * (end.first  - start.first ) / points);
        y = (unsigned  long) round(start.second + i * (end.second - start.second) / points);
        line.push_back(make_pair(x, y));
    }
    line.push_back(end);
    return line;
}

void GA::solve(Grid* grid, vector<coord> &line) {
    if(line.empty())
        throw invalid_argument("line was empty: it should contain at least 2 points");

    // Grid
    this->grid = grid;
    Projection id = Projection::identity();

    // Store begin and end
    this->start = line.front();
    this->end = line.back();

    // Make straight line
    line = straightLine(this->start, this->end, this->points);

    FitFunc fitness = [grid,this,id](const double *x, const int N)->double {
        double val = 0.0;
        double angle = grid->angle(get<0>(this->start), get<1>(this->start), x[0], x[1], id);
        // From windmill
        val += grid->cost(get<0>(this->start), get<1>(this->start), x[0], x[1], id, true);
        for (int i = 0; i+3 < N; i+=4){
            // Distance
            val += grid->cost(x[i], x[i+1], x[i+2], x[i+3], id, true);
            // Angle
            double new_angle = grid->angle(x[i], x[i+1], x[i+2], x[i+3], id);
            val += grid->cost(angle - new_angle);
            angle = new_angle;
        }
        // To windmill
        double new_angle = grid->angle(x[N-2], x[N-1], get<0>(this->end), get<1>(this->end), id);
        val += grid->cost(x[N-2], x[N-1], get<0>(this->end), get<1>(this->end), id, true);
        val += grid->cost(angle - new_angle, true);
        cout << "Fitness: " << val << endl;
        return val;
    };

    // problem dimensions: this->points
    vector<double> x0(this->points*2);
    for(vector<coord>::const_iterator it = line.begin(); it < line.end(); it++){
        x0.push_back(it->first);
        x0.push_back(it->second);
    }

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