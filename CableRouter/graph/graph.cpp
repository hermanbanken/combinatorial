#include "graph.h"
#include <list>
#include <queue>
#include <vector>
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


bool const CompareDijkstra::operator()(pair<unsigned long, double> &nodeX, pair<unsigned long, double> &nodeY) {
    return (nodeX.second > nodeY.second) ;
}

double Graph::dijkstra(Node &from, Node &to, vector<coordinate> &line) {
    Node *u;
    pair<unsigned long, double> p;
    Node *to_node;
    double alt;
    double dist[this->nodes.size()];
    bool scanned[this->nodes.size()];
    unsigned long previous[this->nodes.size()];
    priority_queue<vector<pair<unsigned long, double>>, vector<pair<unsigned long, double>>, CompareDijkstra> pq;

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

double Graph::aStar(Graph::Node &start, Graph::Node &goal, vector<coordinate > &line) {
    bool closed[this->nodes.size()], found = false;
    priority_queue<vector<pair<unsigned long, double>>, vector<pair<unsigned long, double>>, CompareDijkstra> open;
    unsigned long came_from[this->nodes.size()];
    double g_score[this->nodes.size()], f_score[this->nodes.size()], tentative_g_score;
    Node *current, *neighbour;
    list<Edge>::iterator edge;

    std::fill_n(closed, this->nodes.size(), false);
    std::fill_n(g_score, this->nodes.size(), DBL_MAX);

    g_score[start.id] = 0;
    f_score[start.id] = aStarCostEstimate(start, goal);

    open.emplace(make_pair(start.id, f_score[start.id]));

    while(!open.empty()) {
        current = &this->nodes.at(open.top().first); open.pop();
//        cout << "Current node is " << current->id << endl;
        if (closed[current->id])
            continue;

        if (current->id == goal.id) {
            found = true;
            break;
        }

        closed[current->id] = true;
        for (edge = current->adjEdges.begin(); edge != current->adjEdges.end(); ++edge) {
            if (edge->from->id == current->id)
                neighbour = edge->to;
            else
                neighbour = edge->from;

            if (closed[neighbour->id])
                continue;

            // updateNode
            tentative_g_score = g_score[current->id] + edge->weight;

            if (tentative_g_score < g_score[neighbour->id])
            {
                came_from[neighbour->id] = current->id;
                g_score[neighbour->id] = tentative_g_score;
                f_score[neighbour->id] = tentative_g_score + aStarCostEstimate(*neighbour, goal);
                open.emplace(make_pair(neighbour->id, f_score[neighbour->id]));
            }
        }
    }

    if(!found)
        return -1;

    current = &goal;
    line.clear();
    while(true)
    {
//        cout << current->p.first << "," << current->p.second << endl;
        line.push_back(current->p);

        if (current->id == start.id)
            break;

        current = &this->nodes[came_from[current->id]];
    }

    return g_score[goal.id];
}

double Graph::aStarCostEstimate(Graph::Node &from, Graph::Node &to) {
    return EUCL(from.p.first, from.p.second, to.p.first, to.p.second);
}

double Graph::thetaStar(Graph::Node &start, Graph::Node &goal, Grid &grid, Projection &projection, vector<coordinate > &line) {
    bool closed[this->nodes.size()], found = false;
    priority_queue<vector<pair<unsigned long, double>>, vector<pair<unsigned long, double>>, CompareDijkstra> open;
    unsigned long came_from[this->nodes.size()];
    double g_score[this->nodes.size()], f_score[this->nodes.size()], tentative_g_score;
    Node *current, *neighbour;
    list<Edge>::iterator edge;
    Projection p = projection.back();

    std::fill_n(closed, this->nodes.size(), false);
    std::fill_n(g_score, this->nodes.size(), DBL_MAX);

    g_score[start.id] = 0;
    f_score[start.id] = aStarCostEstimate(start, goal);
    came_from[start.id] = start.id;

    open.emplace(make_pair(start.id, f_score[start.id]));

    while(!open.empty()) {
        current = &this->nodes.at(open.top().first); open.pop();
//        cout << "Current node is " << current->id << endl;
        if (closed[current->id])
            continue;

        if (current->id == goal.id) {
            found = true;
            break;
        }

        closed[current->id] = true;
        for (edge = current->adjEdges.begin(); edge != current->adjEdges.end(); ++edge) {
            if (edge->from->id == current->id)
                neighbour = edge->to;
            else
                neighbour = edge->from;

            if (closed[neighbour->id])
                continue;

            if (grid.lineOfSight(
                    nodes[came_from[current->id]].p.first, nodes[came_from[current->id]].p.second,
                    nodes[neighbour->id].p.first, nodes[neighbour->id].p.second,
                    p)) {
                // Path 2
                tentative_g_score = g_score[came_from[current->id]] \
                        + grid.cost(nodes[came_from[current->id]].p.first, nodes[came_from[current->id]].p.second,
                        nodes[neighbour->id].p.first, nodes[neighbour->id].p.second,
                        p, true);

                if (tentative_g_score < g_score[neighbour->id]) {
                    came_from[neighbour->id] = came_from[current->id];
                    g_score[neighbour->id] = tentative_g_score;
                    f_score[neighbour->id] = tentative_g_score + aStarCostEstimate(*neighbour, goal);
                    open.emplace(make_pair(neighbour->id, f_score[neighbour->id]));
                }

                continue;
            }

            // Path 1
            tentative_g_score = g_score[current->id] + edge->weight;

            if (tentative_g_score < g_score[neighbour->id])
            {
                came_from[neighbour->id] = current->id;
                g_score[neighbour->id] = tentative_g_score;
                f_score[neighbour->id] = tentative_g_score + aStarCostEstimate(*neighbour, goal);
                open.emplace(make_pair(neighbour->id, f_score[neighbour->id]));
            }
        }
    }

    if(!found)
        return -1;

    current = &goal;
    line.clear();
    while(true)
    {
//        cout << current->p.first << "," << current->p.second << endl;
        line.push_back(current->p);

        if (current->id == start.id)
            break;

        current = &this->nodes[came_from[current->id]];
    }

    return g_score[goal.id];
}
