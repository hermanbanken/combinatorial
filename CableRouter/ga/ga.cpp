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
    vector<coordinate> line(points);
    double x = 0;
    double y = 0;

    // TODO: fix line to be evenly spaced
    for(unsigned int i = 0; i < points; i++){
        x = start.first  + 1.0 * (i+1) * (1.0 * end.first  - start.first ) / points;
        y = start.second + 1.0 * (i+1) * (1.0 * end.second - start.second) / points;
        line[i] = make_pair(x, y);
    }
    return line;
}

string p2s(coord c){
    std::ostringstream stringStream;
    stringStream << c.first << "," << c.second;
    return stringStream.str();
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
    line.push_back(make_pair(start.first, start.second));
    for(unsigned int i = 0; i < c.get_x_size(); i+=2){
        line.push_back(make_pair(c.get_x()[i], c.get_x()[i+1]));
    }
    line.push_back(make_pair(end.first, end.second));
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
//        ostringstream out;
//        out << "Line from start " << p2s(this->start);
        double val = 0.0;
        double angle = grid->angle(this->start.first, this->start.second, x[0], x[1], id);
        // From windmill
        val += grid->cost(get<0>(this->start), get<1>(this->start), x[0], x[1], id, true);
//        out << " to " << x[0] << "," << x[1];
        for (int i = 0; i+3 < N; i+=2){
//            out << " to " << x[i+2] << "," << x[i+3];
            // Distance
            val += grid->cost(x[i], x[i+1], x[i+2], x[i+3], id, true);
            // Angle
            double new_angle = grid->angle(x[i], x[i+1], x[i+2], x[i+3], id);
            val += grid->cost(grid->angle(angle, new_angle), true);
            angle = new_angle;
        }
//        out << " to " << p2s(this->end) << "; fitness: ";
        // To windmill
        double new_angle = grid->angle(x[N-2], x[N-1], this->end.first, this->end.second, id);
        val += grid->cost(x[N-2], x[N-1], this->end.first, this->end.second, id, true);
        val += grid->cost(grid->angle(angle, new_angle), true);
//        cout << out.str() << val << endl;
        return val;
    };

    // Config
    double sigma = 50;
    vector<double> x0 = lineCandidate(line);
    //int lambda = 100; // offsprings at each generation.
    CMAParameters<> cmaparams(this->points*2,&x0.front(),sigma);
    //cmaparams.set_algo(BIPOP_CMAES);
    CMASolutions cmasols = cmaes<>(fitness,cmaparams);
    cout << "best solution: " << cmasols << endl;
    cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds" << endl;
    int o = cmasols.run_status();
    cout << "Result: " << o << endl;

    // Return
    line = candidateLine(this->start, cmasols.best_candidate(), this->end);
}