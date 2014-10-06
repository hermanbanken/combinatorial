#include <list>
#include "../structures.h"

class Graph {
public:
    struct Node;
    struct Edge;

    list<Node> nodes;
    list<Edge> edges;

    struct Node {
        cell p;
        long long id;
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