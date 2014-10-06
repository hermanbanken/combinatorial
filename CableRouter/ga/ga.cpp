#include "ga.h"
#include <tuple>
#include <iostream>
#include <vector>
#include "../solver.h"
#include <stdexcept>
#include "cmaes.h"
#include <math.h>

using namespace std;
using namespace libcmaes;
using namespace Solvers;

Solvers::GA::GA(int points) {
    this->points = points;
}

double Solvers::GA::cost(double ax, double ay, double bx, double by, float &angle) {
    double val = 0;

    /* Angles */
    float new_angle = atan2(by - ay, bx - ax);
    // Initial angle is FLT_MIN
    if(angle > FLT_MIN) {
        val = abs(angle - new_angle) - ALLOWED_ANGLE > 0 ? DBL_MAX : val;
    }
    angle = new_angle;

    /* Distance */
    double distance = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)) * CABLE_COST;
    val = val + distance < val ? val : val + distance; // can be max value

    /* Obstacles and off map */
    for(double p = 0; p < distance; p++){
        double cx = ax + bx * p / distance * (ax < bx ? 1 : -1);
        double cy = ay + by * p / distance * (ay < by ? 1 : -1);

        // Off map
        if(cx < 0 || cx > this->grid.size())
            val = DBL_MAX;
        else if(cy < 0 || cy > this->grid.begin()->size())
            val = DBL_MAX;
        else {
            cell c = this->grid[(unsigned long)cx][(unsigned long)cy];
            if(!c.mapped)
                val = DBL_MAX;
            else {
                // obstacles
            }
        }
    }

    return val;
}

void GA::solve(vector<vector<cell>> &grid, vector<tuple<unsigned long, unsigned long>> &line) {
    if(line.empty())
        throw invalid_argument("line was empty: it should contain at least 2 points");

    // Grid
    this->grid = grid;

    // Move last point to correct position
    this->start = line.front();
    this->end = line.back();
    line.erase (line.end());
    line.reserve((unsigned long) this->points+2);
    line.insert(line.end(), this->end);

    FitFunc fitness = [this](const double *x, const int N)->double {
        double val = 0.0;
        float angle = FLT_MIN;
        // From windmill
        val += this->cost(get<0>(this->start), get<1>(this->start), x[0], x[1], angle);
        for (int i = 0; i+3 < N; i+=4)
            val += this->cost(x[i], x[i+1], x[i+2], x[i+3], angle);
        // To windmill
        val += this->cost(x[N-2], x[N-1], get<0>(this->end), get<1>(this->end), angle);
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