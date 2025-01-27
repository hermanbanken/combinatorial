#include "../structures.h"
#include "graph.h"
#include <list>
#include <vector>
#include <tuple>

#ifndef ___cablerouter_sampler___
#define ___cablerouter_sampler___

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
    Projection projection;
    Graph graph;
    Sampler(Grid *grid);
    void sample(int noPoints);
    void sampleGrid(int noPoints, list<coordinate>& points);

    void createNodes(list<coordinate> &points);

    void createAllEdges();

    Graph::Node findNearestNode(coordinate &param);

    void sample8ConnectedGridNodesAndEdges(int noPoints);

    void createEdge(Graph::Node *from, Graph::Node *to);
};

#endif /* defined(___cablerouter_sampler___) */