#include "../structures.h"
#include "anglethetastarsolver.h"
#include "sampler.h"

using namespace std;


void Solvers::AngleAwareThetaStarSolver::preprocess(Grid *grid, double &time) {
    clock_t start = clock();

    grid->floodFindDistancesToEdge();

    sampler = new Sampler(grid);
    sampler->sample8ConnectedGridNodesAndEdges(this->noPoints);

    time = double(clock() - start);
}

void Solvers::AngleAwareThetaStarSolver::solve(Grid *grid, vector<coordinate> &line, double &time) {
    clock_t start = clock();

    Graph::Node from = sampler->findNearestNode(line.front());
    Graph::Node to = sampler->findNearestNode(line.back());

    double distance = sampler->graph.angleAwareThetaStar(from, to, *grid, sampler->projection, line);

    time = double(clock() - start);
}


Solvers::AngleAwareThetaStarSolver::AngleAwareThetaStarSolver(int noPoints) {
    this->noPoints = noPoints;
}