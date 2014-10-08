#include <iostream>
#include "structures.h"
#include "reader.h"
#include "simplifier.h"
#include "ga/ga.h"
#include "graph/dijkstrasolver.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEFAULT_GRID_SIZE 2;
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
        cout << "Read " << simplifier->count << " points\n";
        cout << "Min:\n\t" << simplifier->min->toString() << "\n";
        cout << "Max:\n\t" << simplifier->max->toString() << "\n";
        cout << "Avg:\n\t" << simplifier->avg->toString() << "\n";

        // End timer
        clock_t stop = clock();
        cout << double(stop - start) / CLOCKS_PER_SEC << " seconds for input\n";

        grid = simplifier->grid(grid_s, grid_s);
        clock_t stop2 = clock();
        cout << double(stop2 - stop) / CLOCKS_PER_SEC << " seconds for grid\n";

//        grid->write(bin);
    }

//    grid->summary(cout);
    grid->plot(cout);

    Projection id = Projection::identity();

    vector<coord> line(2, make_pair(7, grid->maxY(id) - 3));
    line[1] = make_pair(grid->maxX(id) - 2, 2);

    Solvers::DijkstraSolver* g = new Solvers::DijkstraSolver();
    g->solve(grid, line);

    //Solvers::GA* g = new Solvers::GA(GA_COMPLEXITY);
    //g->solve(grid, line);

    return 0;
}

