#import <list>
#include <queue>
#import "graph.h"

void Graph::addNode(Node &node) {
    node.id = this->nodes.size();
    this->nodes.push_back(node);
}

void Graph::addEdge(Edge &edge) {
    this->edges.push_back(edge);
}

Graph::Edge Graph::connect(Node &from, Node &to) {
    Edge edge{&from, &to, 1};

    this->addEdge(edge);
    return edge;
}

class CompareGreater {
public:
    bool const operator()(Vertices &nodeX, Vertices &nodeY) {
        return (nodeX.dist > nodeY.dist) ;
    }
};

double Graph::shortestPath(Node &from, Node &to) {
    Node u;
    double alt;
    double dist[this->nodes.size()];
    bool scanned[this->nodes.size()];
    priority_queue<Node, vector, CompareGreater> pq;
    std::list<Node>::const_iterator iterator;

    std::fill_n(dist, this->nodes.size(), std::numeric_limits<double>::infinity());
    dist[from.id] = 0;
    from.dist = 0;

    pq.push(from, 0);
    while (!pq.empty()) {
        u = pq.top(); pq.pop();
        scanned[u.id] = true;

        for (iterator = u.adjEdges.begin(); iterator != u.adjEdges.end(); ++iterator) {
            if (!scanned[*iterator.to.id])
            {
                alt = dist[u.id] + *iterator.weight;
                if (*iterator.to.id == to.id)
                {
                    return alt;
                }

                if (alt < dist[*iterator.to.id])
                {
                    dist[*iterator.to.id] = alt;
                    pq.push(*iterator.to);
                }
            }
        }
    }

    return dist[to.id];
}