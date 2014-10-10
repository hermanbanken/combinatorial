#include <list>
#include "../structures.h"

#ifndef ___cablerouter_graph___
#define ___cablerouter_graph___

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
    double shortestPath(Node &from, Node &to);
};

#endif /* defined(___cablerouter_graph___) */
