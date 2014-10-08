#include <list>
#include <tuple>
#include <vector>
#include <iostream>
#include "sampler.h"
#include "../structures.h"
#include "graph.h"

#define NO_NODES 4000

using namespace std;

typedef pair<float, float> coordinate;

Sampler::Sampler(Grid *grid) : grid(grid) {
    this->graph = Graph();
}

void Sampler::sample() {
    list<coordinate> points;

    cout << "Sampling:" << endl;
    clock_t start = clock();
    sampleGrid(NO_NODES, points);
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

    Projection p = this->grid->to_ZeroToOne_Projection().back();

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
    float distX, distY;

    noPointsX = (int) sqrt(noPoints);
    noPointsY = (int) sqrt(noPoints);

    distX = 1.0f / noPointsX;
    distY = 1.0f / noPointsY;

    for (x = 0; x <= noPointsX; x++) {
        for (y = 0; y < noPointsY; y++) {
            points.push_back(coordinate(distX * x, distY * y));
        }
    }
}


void Sampler::createAllEdges() {
    Graph::Edge *e;
    Projection p = this->grid->to_ZeroToOne_Projection().back();

    for (vector<Graph::Node>::iterator it_from = this->graph.nodes.begin(); it_from != this->graph.nodes.end(); ++it_from) {
        for (vector<Graph::Node>::iterator it_to = this->graph.nodes.begin(); it_to != this->graph.nodes.end(); ++it_to) {
            if (it_from->id == it_to->id)
                continue;


            e = new Graph::Edge();
            e->from = &*it_from;
            e->to = &*it_to;
            e->weight = grid->cost(e->from->p.first, e->from->p.second, e->to->p.first, e->to->p.second, p);

            if (e->weight != DBL_MAX) {
                graph.addEdge(*e);
            }
        }
    }
}

Graph::Node Sampler::findNearestNode(coord &point) {
    double x, y;
    double distance, max_distance;
    Graph::Node nearest;

    this->grid->to_ZeroToOne_Projection().project(point.first, point.second, x, y);

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
