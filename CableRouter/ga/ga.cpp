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

vector<coordinate> splitMaxSegment(vector<coordinate> line, Grid* grid, Projection &p){
    unsigned int mI = 0;
    double max = grid->cost(line[0].first, line[0].second, line[1].first, line[1].second, p, true);
    for(unsigned int i = 1; i + 1 < line.size(); i++){
        double c = grid->cost(line[i].first, line[i].second, line[i+1].first, line[i+1].second, p, true);
        if(c > max){
            mI = i;
            max = c;
        }
    }
    // Add point halfway the most expensive line part
    line.insert(line.begin() + mI + 1, make_pair((line[mI].first + line[mI+1].first)/2, (line[mI].second + line[mI+1].second)/2));
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
        return grid->cost(this->start, x, N, this->end, id);
    };

    for(int tries = 1; tries > 0; tries--)
    for(unsigned int complexity = 0; complexity <= this->points; complexity++){
        // Make straight line
        if(complexity == 0){
            line = straightLine(this->start, this->end, complexity);
            results[complexity].set_fvalue(grid->cost(line, id));
        } else {
            // Introduce one point extra, increasing complexity
            line = splitMaxSegment(line, grid, id);

            // Config
            vector<double> x0 = lineCandidate(line);
            CMAParameters<> cmaparams(complexity*2,&x0.at(2), 50);
            cmaparams.set_mt_feval(true); // multi threading

            // Run
            CMASolutions cmasols = cmaes<>(fitness,cmaparams);

            if(results.count(complexity) == 0 || results[complexity].get_fvalue() > cmasols.best_candidate().get_fvalue())
                results[complexity] = cmasols.best_candidate();
        }
    }

    Candidate best = results.begin()->second;
    for(map<unsigned int, ::libcmaes::Candidate>::iterator i = results.begin(); i != results.end(); i++){
        best = best.get_fvalue() < i->second.get_fvalue() ? best : i->second;
    }

    line = candidateLine(this->start, best, this->end);

    time = double(clock() - start);
}