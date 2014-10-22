#include "../structures.h"
#include "astarsolver.h"
#include "sampler.h"

using namespace std;

void Solvers::AStarSolver::preprocess(Grid *grid, double &time) {
    clock_t start = clock();

    grid->floodFindDistancesToEdge();

    sampler = new Sampler(grid);
    sampler->sample8ConnectedGridNodesAndEdges(this->noPoints);

    time = double(clock() - start);
}

void Solvers::AStarSolver::solve(Grid *grid, vector<coordinate> &line, double &time) {
    clock_t start = clock();

    Graph::Node from = sampler->findNearestNode(line.front());
    Graph::Node to = sampler->findNearestNode(line.back());

    double distance = sampler->graph.aStar(from, to, line);

    time = double(clock() - start);
}


Solvers::AStarSolver::AStarSolver(int noPoints) {
    this->noPoints = noPoints;
}
