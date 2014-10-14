#include <list>
#include "../structures.h"

#ifndef ___cablerouter_graph___
#define ___cablerouter_graph___

class CompareDijkstra {
public:
    bool const operator()(pair<unsigned long, double> &nodeX, pair<unsigned long, double> &nodeY);
};

class Graph {
public:
    struct Node;
    struct Edge;

    vector<Node> nodes;
    vector<Edge> edges;

    struct Node {
        coordinate p;
        unsigned long id;
        list <Edge> adjEdges;
        double dist;
    };

    struct Edge {
        Graph::Node * from;
        Graph::Node * to;
        double weight;
    };

    void addNode(Node &node);
    void addEdge(Edge &edge);
    Edge connect(Node &from, Node &to);
    double dijkstra(Node &from, Node &to, vector<coordinate> &line);

    // A*
    double aStar(Node &start, Node &goal, vector<coordinate> &line);
    double aStarCostEstimate(Graph::Node &from, Graph::Node &to);

    // Theta*
    double thetaStar(Node &start, Node &goal, Grid &grid, Projection &p, vector<coordinate> &line);
    double angleAwareThetaStar(Node &start, Node &goal, Grid &grid, Projection &p, vector<coordinate> &line);
};

#endif /* defined(___cablerouter_graph___) */
