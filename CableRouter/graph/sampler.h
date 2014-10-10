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
    Projection projection;
public:
    Graph graph;
    Sampler(Grid *grid);
    void sample();
    void sampleGrid(int noPoints, list<coordinate>& points);

    void createNodes(list<coordinate> &points);

    void createAllEdges();

    Graph::Node findNearestNode(coordinate &param);
};