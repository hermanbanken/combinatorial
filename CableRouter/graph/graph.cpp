#include "graph.h"
#include <list>
#include <queue>
#include <iostream>

void Graph::addNode(Node &node) {
    node.id = this->nodes.size();
    this->nodes.push_back(node);
}

void Graph::addEdge(Edge &edge) {
    this->edges.push_back(edge);
    edge.from->adjEdges.push_back(edge);
    edge.to->adjEdges.push_back(edge);
}

Graph::Edge Graph::connect(Node &from, Node &to) {
    Edge edge{&from, &to, 1};

    this->addEdge(edge);
    return edge;
}

class CompareGreater {
public:
    bool const operator()(Graph::Node &nodeX, Graph::Node &nodeY) {
        return (nodeX.dist > nodeY.dist) ;
    }
};

double Graph::shortestPath(Node &from, Node &to) {
    Node u;
    Node *to_node;
    double alt;
    double dist[this->nodes.size()];
    bool scanned[this->nodes.size()];
    priority_queue<Node, vector<Node>, CompareGreater> pq;

    std::fill_n(dist, this->nodes.size(), DBL_MAX);
    std::fill_n(scanned, this->nodes.size(), false);
    dist[from.id] = 0;
    from.dist = 0;

//    std::cout << "Planning: " << from.id << " -> " << to.id << endl;
//    std::cout << "coordinates: (" << from.p.first << "," << from.p.second << ") -> (" << to.p.first << "," << to.p.second << ")" << endl;

    pq.emplace(from);
    while (!pq.empty()) {
        u = pq.top(); pq.pop();
        scanned[u.id] = true;
//        std::cout << "Node id: " << u.id << endl;
//        std::cout << "No. Adj edges: " << u.adjEdges.size() << endl;
        for (list<Edge>::iterator it = u.adjEdges.begin(); it != u.adjEdges.end(); ++it) {
            if (it->from->id == u.id)
                to_node = it->to;
            else
                to_node = it->from;
//            std::cout << "scanned node " << to_node->id << "? " << scanned[to_node->id] << endl;
            if (!scanned[to_node->id])
            {
                alt = dist[u.id] + it->weight;
//                std::cout << "alt: " << alt << ", old_dist: " << dist[to_node->id] <<  endl;

                if (alt < dist[to_node->id])
                {
                    dist[to_node->id] = alt;
                    pq.emplace(*to_node);
//                    std::cout << "Emplaced node " << to_node->id << endl;
                }
            }
        }
    }

    if (dist[to.id] == DBL_MAX)
        return -1;

    return dist[to.id];
}