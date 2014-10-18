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
#define DEFAULT_GRID_SIZE 10;
const int GA_COMPLEXITY = 5;

using namespace std;

// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool exists_test (const std::string& name) {
    return (-1 != access( name.c_str(), F_OK ));
}

int actual(int argc, char const *argv[]) {
    // Start timer
    clock_t start = clock();

    double grid_s = argc >= 3 ? atof(argv[2]) : DEFAULT_GRID_SIZE;
    string file = argc >= 2 ? argv[1] : "/Projects/Combinatorial/data/matrix 2.txt";
    int complexity = argc >= 4 ? atoi(argv[3]) : GA_COMPLEXITY;
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

    vector<coordinate> line(2, make_pair(3.0, grid->maxY(id) - 3.0));
    line[1] = make_pair(grid->maxX(id) - 2.0, 2.0);

    double dist = grid->distance(line[0].first, line[0].second, line[1].first, line[1].second, id);
    cout << "Solving the placement of a cable between two points " << dist << "m apart" << endl;

    if(dist / 50 < GA_COMPLEXITY+1)
        cout << "Warning: the input space might be too small to make " << (GA_COMPLEXITY + 1) << " turns." << endl;

//    Solvers::DijkstraSolver* g = new Solvers::DijkstraSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

//    Solvers::AStarSolver* g = new Solvers::AStarSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

//    Solvers::ThetaStarSolver* g = new Solvers::ThetaStarSolver(NO_GRAPH_NODES);
//    g->solve(grid, line);

    Solvers::AngleAwareThetaStarSolver* g = new Solvers::AngleAwareThetaStarSolver(NO_GRAPH_NODES);
    g->solve(grid, line);

//    Solvers::GA* g = new Solvers::GA(GA_COMPLEXITY);
//    g->solve(grid, line);

    grid->plot(cout, line);

    return 0;
}

void test() {
    Grid* g = new Grid(vector<vector<cell>>(0), Projection::identity());

    cout << "Testing all angles in circle: ";
    for(float a = 0.0; a <= 2*M_PI; a += M_PI / 8){
        double x = cos(a) * 1;
        double y = sin(a) * 1;
        double r = g->angle(0.0, 0.0, x, y, Projection::identity());
        cout << (fabs(r - a) > 0.001f ? "F" : ".");
        if(fabs(r - a) > 0.001f)
            cout << "\n\t(x:"<<x<<",y:"<<y<<") - Expected: "<< a << "; Got: " << r << endl;
    }
    cout << endl;

    cout << "Testing all angles in 2 circles: ";
    for(float a1 = 0.0; a1 <= 2*M_PI; a1 += M_PI / 8){
        double x = cos(a1) * 1;
        double y = sin(a1) * 1;
        double r1 = g->angle(0.0, 0.0, x, y, Projection::identity());

        for(float a2 = 0.0; a2 <= 2*M_PI; a2 += M_PI / 8){
            double x2 = x + cos(a2) * 1;
            double y2 = y + sin(a2) * 1;
            double r2 = g->angle(x, y, x2, y2, Projection::identity());

            double r = g->angle(r1, r2);

            double diff = r2 - r1;
            diff = fabs(diff);

            if(fabs(r - diff) > 0.001f)
                cout << "F\n\tExpected: "<< diff << "; Got: " << r << endl;
            else
                cout << ".";
        }
    }

    cout << endl;
}

int main(int argc, char const *argv[]) {
    //test();
    //return 0;
    return actual(argc, argv);
}