#include "../structures.h"
#include "dijkstrasolver.h"
#include "sampler.h"

void Solvers::DijkstraSolver::solve(Grid *grid, vector<tuple<unsigned long, unsigned long>> &line) {
    Sampler sampler = Sampler(grid);

    sampler.sample();

    Graph::Node from = sampler.findNearestNode(line.front());
    Graph::Node to = sampler.findNearestNode(line.back());

    double distance = sampler.graph.shortestPath(from, to);

    cout << "best solution: " << distance << endl;
}


