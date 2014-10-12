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
    bool const operator()(pair<unsigned long, double> &nodeX, pair<unsigned long, double> &nodeY) {
        return (nodeX.second > nodeY.second) ;
    }
};

double Graph::dijkstra(Node &from, Node &to, vector<coordinate> &line) {
    Node *u;
    pair<unsigned long, double> p;
    Node *to_node;
    double alt;
    double dist[this->nodes.size()];
    bool scanned[this->nodes.size()];
    unsigned long previous[this->nodes.size()];
    priority_queue<Node, vector<pair<unsigned long, double>>, CompareGreater> pq;

    std::fill_n(dist, this->nodes.size(), DBL_MAX);
    std::fill_n(scanned, this->nodes.size(), false);
    std::fill_n(previous, this->nodes.size(), ((unsigned long) 0)-1);
    dist[from.id] = 0;
    from.dist = 0;

//    std::cout << "Planning: " << from.id << " -> " << to.id << endl;
//    std::cout << "coordinates: (" << from.p.first << "," << from.p.second << ") -> (" << to.p.first << "," << to.p.second << ")" << endl;

    pq.emplace(make_pair(from.id, 0.0));
    while (!pq.empty()) {
        p = pq.top(); pq.pop();
        u = &this->nodes.at(p.first);
        scanned[u->id] = true;
//        std::cout << "Node id: " << u.id << endl;
//        std::cout << "No. Adj edges: " << u.adjEdges.size() << endl;
        for (list<Edge>::iterator it = u->adjEdges.begin(); it != u->adjEdges.end(); ++it) {
            if (it->from->id == u->id)
                to_node = it->to;
            else
                to_node = it->from;
//            std::cout << "scanned node " << to_node->id << "? " << scanned[to_node->id] << endl;
            if (!scanned[to_node->id])
            {
                alt = dist[u->id] + it->weight;
//                std::cout << "alt: " << alt << ", old_dist: " << dist[to_node->id] <<  endl;

                if (alt < dist[to_node->id])
                {
                    dist[to_node->id] = alt;
                    previous[to_node->id] = u->id;
                    pq.emplace(make_pair(to_node->id, alt));
//                    std::cout << "Emplaced node " << to_node->id << endl;
                }
            }
        }
    }

    if (dist[to.id] == DBL_MAX)
        return -1;

    to_node = &to;
    line.clear();
    while(true)
    {
        cout << to_node->p.first << "," << to_node->p.second << endl;
        line.push_back(to_node->p);

        if (to_node->id == from.id)
            break;

        to_node = &this->nodes[previous[to_node->id]];
    }

    return dist[to.id];
}
