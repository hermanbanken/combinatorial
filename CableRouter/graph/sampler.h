#include "../structures.h"
#include "graph.h"
#include <list>
#include <vector>
#include <tuple>

/*
Possibilities for sampling:
    - Random
    + Grid
    + Halton
    - Halton*
    - random Halton
    - cell Based
 */


class Sampler
{
    Grid *grid;
public:
    Graph graph;
    Sampler(Grid *grid);
    void sample();
    void sampleGrid(int noPoints, list<pair<float, float>>& points);

    void createNodes(list<pair<float, float>> &points);

    void createAllEdges();

    Graph::Node findNearestNode(std::tuple<unsigned long, unsigned long> &param);
};