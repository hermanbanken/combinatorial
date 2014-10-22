#include <list>
#include <tuple>
#include <vector>
#include <iostream>
#include "sampler.h"
#include "../structures.h"
#include "graph.h"

using namespace std;

Sampler::Sampler(Grid *grid) : grid(grid), projection(Projection::identity()) {
    this->graph = Graph();
}

void Sampler::sample(int noPoints) {
    list<coordinate> points;

    cout << "Sampling:" << endl;
    clock_t start = clock();
    sampleGrid(noPoints, points);
    clock_t grid_sampling = clock();
    cout << "\t\tpoint sampling took: " << double(grid_sampling - start) / CLOCKS_PER_SEC << " sec" << endl;
    createNodes(points);
    clock_t node_creating = clock();
    cout << "\t\tnode creating took: " << double(node_creating - grid_sampling) / CLOCKS_PER_SEC << " sec" << endl;
    createAllEdges();
    clock_t edge_creating = clock();
    cout << "\t\tedge creating took: " << double(edge_creating - node_creating) / CLOCKS_PER_SEC << " sec" << endl;
    cout << "\ttotal sampling: " << double(edge_creating - start) / CLOCKS_PER_SEC << " sec" << endl;
}

void Sampler::createNodes(list<coordinate> &points) {
    Graph::Node *n;
    cell c;

    Projection p = projection.back();

    for (list<coordinate>::const_iterator it = points.begin(); it != points.end(); ++it) {
        c = this->grid->getCell(it->first, it->second, p);
        if (c.mapped) {
            c.bomb = true;
            n = new Graph::Node();
            n->p = *it;
            this->graph.addNode(*n);
        }
    }
}

void Sampler::sampleGrid(int noPoints, list<coordinate> &points) {
    int noPointsX, noPointsY, x, y;
    double distX, distY;

    noPointsX = (int) sqrt(noPoints);
    noPointsY = (int) sqrt(noPoints);

    // Use normal projection/back-projection here, if changing projection again
    distX = (grid->maxX(projection) - grid->minX(projection)) / noPointsX;
    distY = (grid->maxY(projection) - grid->minY(projection)) / noPointsY;

    for (x = 0; x < noPointsX; x++) {
        for (y = 0; y < noPointsY; y++) {
            points.push_back(coordinate(grid->minX(projection) + distX * x, grid->minY(projection) + distY * y));
        }
    }
}

void Sampler::sample8ConnectedGridNodesAndEdges(int noPoints) {
    int noPointsX, noPointsY, x, y;
    double distX, distY;
    coordinate point;
    Projection p = projection.back();
    Graph::Node *n;


    noPointsX = (int) sqrt(noPoints);
    noPointsY = (int) sqrt(noPoints);

    //cout << "Creating Node Grid" << endl;
    unsigned long nodeGrid[noPointsX][noPointsY];
    bool boolGrid[noPointsX][noPointsY];
    std::fill( &boolGrid[0][0], &boolGrid[0][0] + sizeof(boolGrid) /* / sizeof(flags[0][0]) */, false );

    // Use normal projection/back-projection here, if changing projection again
    distX = (grid->maxX(projection) - grid->minX(projection)) / noPointsX;
    distY = (grid->maxY(projection) - grid->minY(projection)) / noPointsY;

    // Creating nodes
    //cout << "Creating Nodes" << endl;
    for (x = 0; x < noPointsX; x++) {
        for (y = 0; y < noPointsY; y++) {
            point = coordinate(grid->minX(projection) + distX * x, grid->minY(projection) + distY * y);

            if (this->grid->getCell(point.first, point.second, p).mapped) {
                n = new Graph::Node();
                n->p = point;
                this->graph.addNode(*n);
                nodeGrid[x][y] = n->id;
                boolGrid[x][y] = true;
            } else {
                nodeGrid[x][y] = 0;
            }
        }
    }

    //cout << "Creating Edges" << endl;
    for (x = 0; x < noPointsX; x++) {
        for (y = 0; y < noPointsX; y++) {
            if (!boolGrid[x][y]) {
                continue;
            }
            if(x != 0) {
                if (boolGrid[x-1][y]) {
                    createEdge(&graph.nodes[nodeGrid[x][y]], &graph.nodes[nodeGrid[x - 1][y]]);
                }

                if (y != 0) {
                    if (boolGrid[x - 1][y - 1])
                        createEdge(&graph.nodes[nodeGrid[x][y]], &graph.nodes[nodeGrid[x - 1][y - 1]]);
                }
            }
            if(y != 0) {
                if (boolGrid[x][y - 1])
                    createEdge(&graph.nodes[nodeGrid[x][y]], &graph.nodes[nodeGrid[x][y - 1]]);
            }
        }
    }
}


void Sampler::createAllEdges() {
    for (vector<Graph::Node>::iterator it_from = this->graph.nodes.begin(); it_from != this->graph.nodes.end(); ++it_from) {
        for (vector<Graph::Node>::iterator it_to = this->graph.nodes.begin(); it_to != this->graph.nodes.end(); ++it_to) {
            if (it_from->id == it_to->id)
                continue;
            createEdge(&*it_from, &*it_to);
        }
    }
}

void Sampler::createEdge(Graph::Node *from, Graph::Node *to) {
    Graph::Edge *e;
    Projection p = projection.back();

//    cout << "Creating edge between (" << from->p.first << "," << from->p.second << ") and (" << to->p.first << "," << to->p.second << ")" << endl;

    e = new Graph::Edge();
    e->from = from;
    e->to = to;
    e->weight = grid->cost(e->from->p.first, e->from->p.second, e->to->p.first, e->to->p.second, p, true);

    if (e->weight != FLT_MAX) {
        graph.addEdge(*e);
    }
}

Graph::Node Sampler::findNearestNode(coordinate &point) {
    double x, y;
    double distance, max_distance;
    Graph::Node nearest;

    projection.project(point.first, point.second, x, y);

    max_distance = DBL_MAX;
    for (vector<Graph::Node>::const_iterator it = this->graph.nodes.begin(); it != this->graph.nodes.end(); ++it) {
        distance = sqrt((x - it->p.first) * (x - it->p.first) + (y - it->p.second) * (y - it->p.second));
        if (distance < max_distance) {
            nearest = *it;
            max_distance = distance;
        }
    }

    return nearest;
}
