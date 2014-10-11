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

vector<coordinate> GA::straightLine(coordinate start, coordinate end, unsigned long points) {
    vector<coordinate> line(0);
    double x = 0;
    double y = 0;
    line.push_back(start);
    double a = ANGL(start.first, start.second, end.first, end.second);
    double d = EUCL(start.first, start.second, end.first, end.second);
    for(unsigned int i = 0; i < points; i++){
        x = start.first  + cos(a) * (d / (points+1) * i+1);
        y = start.second + sin(a) * (d / (points+1) * i+1);
        line.push_back(make_pair(x,y));
    }
    line.push_back(end);
    return line;
}

string p2s(coord c){
    std::ostringstream stringStream;
    stringStream << c.first << "," << c.second;
    return stringStream.str();
}

double log(string n, double x){
//    cout << n << ": " << x << endl;
    return x;
}

vector<double> lineCandidate(vector<coordinate> line){
    vector<double> x0(0);
    for(vector<coordinate>::const_iterator it = line.begin(); it < line.end(); it++){
        x0.push_back(it->first);
        x0.push_back(it->second);
    }
    return x0;
}

vector<coordinate> candidateLine(coordinate start, Candidate c, coordinate end) {
    vector<coordinate> line(0);
    line.push_back(start);
    for(unsigned int i = 0; i < c.get_x_size(); i+=2){
        line.push_back(make_pair(c.get_x()[i], c.get_x()[i+1]));
    }
    line.push_back(end);
    return line;
}

void GA::solve(Grid* grid, vector<coordinate> &line) {
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
        double angle = grid->angle(this->start.first, this->start.second, x[0], x[1], id);
        // From windmill
        val += log("start line cost", grid->cost(get<0>(this->start), get<1>(this->start), x[0], x[1], id, true));

        for (int i = 0; i+3 < N; i+=2){

            // Distance
            val += log("line cost", grid->cost(x[i], x[i+1], x[i+2], x[i+3], id, true));
            // Angle
            double new_angle = grid->angle(x[i], x[i+1], x[i+2], x[i+3], id);
            val += log("angle cost", grid->cost(grid->angle(angle, new_angle), true));
            angle = new_angle;
        }

        // To windmill
        double new_angle = grid->angle(x[N-2], x[N-1], this->end.first, this->end.second, id);
        val += log("end line cost", grid->cost(x[N-2], x[N-1], this->end.first, this->end.second, id, true));
        val += log("end angle cost", grid->cost(grid->angle(angle, new_angle), true));

        return val;
    };

    // Config
    double sigma = 50;
    vector<double> x0 = lineCandidate(line);
//    //int lambda = 100; // offsprings at each generation.
    CMAParameters<> cmaparams(this->points*2,&x0.at(2),sigma);
    cmaparams.set_mt_feval(true); // multi threading
    cout << "Initial fitness = " << fitness(&x0.at(2), this->points*2) << endl;
    //cmaparams.set_algo(BIPOP_CMAES);

    // Run
    CMASolutions cmasols = cmaes<>(fitness,cmaparams);

    // Print
    cout << "best solution: " << cmasols << endl;
    cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds" << endl;
    int o = cmasols.run_status();
    cout << "Result: " << o << endl;

    // Return
    line = candidateLine(this->start, cmasols.best_candidate(), this->end);
}