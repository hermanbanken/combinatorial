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
    double  a = ANGL(start.first, start.second, end.first, end.second),
            d = EUCL(start.first, start.second, end.first, end.second),
            a_cos = cos(a),
            a_sin = sin(a);

    for(unsigned int i = 1; i <= points; i++){
        x = start.first  + a_cos * (d / (points+1) * i);
        y = start.second + a_sin * (d / (points+1) * i);
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

void GA::solve(Grid* grid, vector<coordinate> &line, double &time) {
    if(line.empty())
        throw invalid_argument("line was empty: it should contain at least 2 points");

    clock_t start = clock();

    // Grid
    this->grid = grid;
    Projection id = Projection::identity();

    // Store begin and end
    this->start = line.front();
    this->end = line.back();

    // Results
    std::map <unsigned int, Candidate> results;

    FitFunc fitness = [grid,this,id](const double *x, const int N)->double {
        vector<coordinate> line(0);
        line.push_back(this->start);
        for(unsigned int i = 0; i < N; i+=2){
            line.push_back(make_pair(x[i], x[i+1]));
        }
        line.push_back(this->end);
        return grid->cost(line, id);
    };

    for(int tries = 2; tries > 0; tries--)
    for(unsigned int complexity = 1; complexity <= this->points; complexity++){
        // Make straight line
        line = straightLine(this->start, this->end, complexity);

        // Config
        vector<double> x0 = lineCandidate(line);
        CMAParameters<> cmaparams(complexity*2,&x0.at(2), 50);
        cmaparams.set_mt_feval(true); // multi threading

        // Run
        CMASolutions cmasols = cmaes<>(fitness,cmaparams);

        if(results.count(complexity) == 0 || results[complexity].get_fvalue() > cmasols.best_candidate().get_fvalue())
            results[complexity] = cmasols.best_candidate();
    }

    Candidate best = results.begin()->second;
    for(map<unsigned int, ::libcmaes::Candidate>::iterator i = results.begin(); i != results.end(); i++){
        best = best.get_fvalue() < i->second.get_fvalue() ? best : i->second;
    }

    // Store, don't neglect straight line costs
    if(best.get_fvalue() > grid->cost(get<0>(this->start), get<1>(this->start), get<0>(this->end), get<1>(this->end), id, true))
        line = straightLine(this->start, this->end, 0);
    else
        line = candidateLine(this->start, best, this->end);

    time = double(clock() - start);
}