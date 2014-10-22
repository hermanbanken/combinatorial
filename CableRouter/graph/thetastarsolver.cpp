#include "../structures.h"
#include "thetastarsolver.h"
#include "sampler.h"

using namespace std;

void Solvers::ThetaStarSolver::preprocess(Grid *grid, double &time) {
    clock_t start = clock();

    grid->floodFindDistancesToEdge();

    sampler = new Sampler(grid);
    sampler->sample8ConnectedGridNodesAndEdges(this->noPoints);

    time = double(clock() - start);
}

void Solvers::ThetaStarSolver::solve(Grid *grid, vector<coordinate> &line, double &time) {
    clock_t start = clock();

    // Lookup start & end nodes
    Graph::Node from = sampler->findNearestNode(line.front());
    Graph::Node to = sampler->findNearestNode(line.back());

    // Shortest path
    double distance = sampler->graph.thetaStar(from, to, *grid, sampler->projection, line);

    time = double(clock() - start);
}


Solvers::ThetaStarSolver::ThetaStarSolver(int noPoints) {
    this->noPoints = noPoints;
}
