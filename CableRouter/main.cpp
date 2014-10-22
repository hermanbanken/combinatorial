#include <iostream>
#include "structures.h"
#include "reader.h"
#include "simplifier.h"
#include "ga/ga.h"
#include "graph/dijkstrasolver.h"
#include "graph/astarsolver.h"
#include "graph/thetastarsolver.h"
#include "graph/anglethetastarsolver.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>


#define DEFAULT_GRID_SIZE 2;
#define NO_GRAPH_NODES 8000
const int GA_COMPLEXITY = 5;

using namespace std;

// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool exists_test (const std::string& name) {
    return (-1 != access( name.c_str(), F_OK ));
}

int main(int argc, char const *argv[]) {
    // Start timer
    clock_t start = clock();

    int grid_s = argc >= 3 ? atoi(argv[2]) : DEFAULT_GRID_SIZE;
    string file = argc >= 2 ? argv[1] : "/Projects/Combinatorial/data/matrix 2.txt";
    string bin = file+".bin";

    Grid* grid;
    if(false){
        Grid g = Grid::read(bin);
        grid = &g;
    } else {
        Reader *r = new Reader(file);
        point *row = new point();
        Simplifier *simplifier = new Simplifier();

        while (r->next(row)) {
            simplifier->add(row);
            row = new point();
        }

        // Summary
//        cout << "Read " << simplifier->count << " points\n";
//        cout << "Min:\n\t" << simplifier->min->toString() << "\n";
//        cout << "Max:\n\t" << simplifier->max->toString() << "\n";
//        cout << "Avg:\n\t" << simplifier->avg->toString() << "\n";

        // End timer
//        clock_t stop = clock();
//        cout << double(stop - start) / CLOCKS_PER_SEC << " seconds for input\n";

        grid = simplifier->grid(grid_s, grid_s);
//        clock_t stop2 = clock();
//        cout << double(stop2 - stop) / CLOCKS_PER_SEC << " seconds for grid\n";
//        grid->write(bin);
    }

//    grid->summary(cout);
//    grid->plot(cout);

    Projection id = Projection::identity();

//    vector<coordinate> line(2, make_pair(3.0, grid->maxY(id) - 3.0));
//    line[1] = make_pair(grid->maxX(id) - 2.0, 2.0);

//    double dist = grid->distance(line[0].first, line[0].second, line[1].first, line[1].second, id);
//    cout << "Solving the placement of a cable between two points " << dist << "m apart" << endl;
//
//    if(dist / 50 < GA_COMPLEXITY+1)
//        cout << "Warning: the input space might be too small to make " << (GA_COMPLEXITY + 1) << " turns." << endl;

//    Solvers::DijkstraSolver* g = new Solvers::DijkstraSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

//    Solvers::AStarSolver* g = new Solvers::AStarSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

//    Solvers::ThetaStarSolver* g = new Solvers::ThetaStarSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

//    Solvers::AngleAwareThetaStarSolver* g = new Solvers::AngleAwareThetaStarSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

//    Solvers::GA* g = new Solvers::GA(GA_COMPLEXITY);
//    g->solve(grid, line);

    string file2 = "/Users/jeffrey/Downloads/route.txt";

    ifstream in(file2);
    if (!in.good()){
        cout << "The file is not readable.";
        exit(0);
    }

    double x, y, cablelength;
    pair<double, double> point1, point2, newpoint;
    double dx, dy, newdx, newdy;
    vector<coordinate> line;
    while ( in >> x >> y >> cablelength )
    {
//        x -= grid->inputProjection().offset_x;
//        y -= grid->inputProjection().offset_y;
        grid->inputProjection().project(x, y, x, y);

        newpoint = make_pair(x, y);
        if(line.size() < 2) {
//            cout << "(" << x << "," << y << ")" << endl;
            line.push_back(newpoint);
            continue;
        }

        point1 = line.at(line.size()-2);
        point2 = line.at(line.size()-1);

        dx = point2.first - point1.first;
        dy = point2.second - point1.second;

        newdx = newpoint.first - point2.first;
        newdy = newpoint.second - point2.second;

        if (fabs(ANGL(point1.first, point1.second, point2.first, point2.second)- ANGL(point2.first, point2.second, newpoint.first, newpoint.second)) < 0.01f)
            line.at(line.size()-1) = newpoint;
        else
//            cout << "(" << x << "," << y << ")" << endl;
            line.push_back(newpoint);
    }

    grid->plot(cout, line);

    return 0;
}
