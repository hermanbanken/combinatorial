#include "../structures.h"
#include "dijkstrasolver.h"
#include "sampler.h"

using namespace std;

void Solvers::DijkstraSolver::solve(Grid *grid, vector<coordinate> &line, double &time) {
    clock_t start = clock();

    grid->floodFindDistancesToEdge();
    Sampler sampler = Sampler(grid);

    sampler.sample(this->noPoints);

    cout << "Find nodes in graph: " << endl;
    clock_t start_find = clock();
    Graph::Node from = sampler.findNearestNode(line.front());
    Graph::Node to = sampler.findNearestNode(line.back());
    clock_t found = clock();
    cout << "\ttook: " << double(found - start_find) / CLOCKS_PER_SEC << " sec" << endl;

    cout << "Find shortest path: " << endl;
    double distance = sampler.graph.dijkstra(from, to, line);

    clock_t path_found = clock();
    cout << "\ttook: " << double(path_found - found) / CLOCKS_PER_SEC << " sec" << endl;

    cout << "best solution: " << distance << endl;
    cout << "Found solution in: " << double(path_found - start) / CLOCKS_PER_SEC << " sec" << endl;
}


Solvers::DijkstraSolver::DijkstraSolver(int noPoints) {
    this->noPoints = noPoints;
}
