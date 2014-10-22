#include "../structures.h"
#include "dijkstrasolver.h"
#include "sampler.h"

using namespace std;

void Solvers::DijkstraSolver::preprocess(Grid *grid, double &time) {
    clock_t start = clock();

    grid->floodFindDistancesToEdge();

    sampler = new Sampler(grid);
    sampler->sample(this->noPoints);

    time = double(clock() - start);
}

void Solvers::DijkstraSolver::solve(Grid *grid, vector<coordinate> &line, double &time) {
    clock_t start = clock();

    // Find nodes
    Graph::Node from = sampler->findNearestNode(line.front());
    Graph::Node to = sampler->findNearestNode(line.back());

    // Solve
    double distance = sampler->graph.dijkstra(from, to, line);

    time = double(clock() - start);
}


Solvers::DijkstraSolver::DijkstraSolver(int noPoints) {
    this->noPoints = noPoints;
}
