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

#define NO_GRAPH_NODES 8000
#define DEFAULT_GRID_SIZE 10;
#define TTY if (isatty(fileno(stdin)))

const int GA_COMPLEXITY = 8;

using namespace std;

// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool exists_test (const std::string& name) {
    return (-1 != access( name.c_str(), F_OK ));
}

int actual(int argc, char const *argv[]) {
    std::cout << "> CableRouter" << endl;

    ASKFILE:do {

        // Load Matrix
        TTY std::cout << "> Enter matrix file name: ";
        string file;
        std::getline(std::cin, file);
        std::cout << "> Reading " << file << endl << flush;

        Reader *r = new Reader(file);
        point *row = new point();
        Simplifier *simplifier = new Simplifier();

        while (r->next(row)) {
            simplifier->add(row);
            row = new point();
        }

        char action = '_';
        char algorithm = 'g';
        Grid* grid;
        float grid_s = 1;
        Solvers::Solver* solver;
        unsigned int num_nodes = NO_GRAPH_NODES;
        unsigned int complexity = GA_COMPLEXITY;
        double time_preprocess = 0;

        do {

            switch(action) {
                case 'c': goto ASKFILE;
                case 'r':
                    if(grid == NULL)
                        grid = simplifier->grid(grid_s, grid_s);

                    // Run algorithms
                    TTY std::cout << "> Which algorithm to run: " << endl <<
                            "> - [g] CMA-ES"             << endl <<
                            "> - [d] Dijkstra"           << endl <<
                            "> - [s] A*"                 << endl <<
                            "> - [t] Theta*"             << endl <<
                            "> - [a] Theta* with angles" << endl <<
                            "> Do [g]: ";
                    std::cin >> algorithm;
                    std::cout << endl;

                    if(algorithm != 'g'){
                        TTY std::cout << "> Parameter: [number of nodes]";
                        std::cin >> num_nodes;
                    } else {
                        TTY std::cout << "> Parameter <max corners>: [ex: " << complexity << "]";
                        std::cin >> complexity;
                    }
                    std::cout << endl;

                    std::cout << "> Using solver ";
                    switch(algorithm){
                        case 'd':   cout << "Dijkstra" << endl;
                                    solver = new Solvers::DijkstraSolver(num_nodes); break;
                        case 's':   cout << "A*" << endl;
                                    solver = new Solvers::AStarSolver(num_nodes); break;
                        case 't':   cout << "Theta*" << endl;
                                    solver = new Solvers::ThetaStarSolver(num_nodes); break;
                        case 'a':   cout << "Theta* with angles" << endl;
                                    solver = new Solvers::AngleAwareThetaStarSolver(num_nodes); break;
                        default:    cout << "CMA-ES" << endl;
                                    solver = new Solvers::GA(complexity);
                    }

                    solver->preprocess(grid, time_preprocess);

                    do {
                        double ax, ay, bx, by, time_solve = 0, fitness;
                        TTY std::cout << "> Cable start and end coordinates, format <a_x> <a_y> <b_x> <b_y>:";

                        if((std::cin >> ax >> ay >> bx >> by) && std::cin.good()){
                            std::cout << endl << "> Finding line from (" << ax << "," << ay << ") to (" << bx << "," << by << ")" << endl << flush;
                            vector<coordinate> line(2, make_pair(ax, ay));
                            line[1] = make_pair(bx, by);
                            std::cout << "> Running... " << endl << flush;
                            solver->solve(grid, line, time_solve);
                            fitness = grid->cost(line, Projection::identity());
                            std::cout << "> Done" << endl;

                            cout << "=" << algorithm << ' ' << fitness << ' ' << time_preprocess <<  ' ' << time_solve << endl;
                            cout << "?(" << line[0].first << "," << line[0].second << ")";
                            for(unsigned int i = 1; i < line.size(); i++) {
                                cout << "->(" << line[i].first << "," << line[i].second << ")";
                            }
                            cout << endl;

                            TTY std::cout << "> Continue? (else hit ctrl-D)" << endl;
                        } else {
                            std::cin.clear();
                            break;
                        }
                    } while(true);

                    break;

                case 's':
                    if(grid == NULL)
                        grid = simplifier->grid(grid_s, grid_s);

                    // Write statistics
                    cout << "{ \"points\": " << simplifier->count << ", " <<
                              "\"min\": \"" << simplifier->min->toString() << "\", "
                              "\"max\": \"" << simplifier->max->toString() << "\", "
                              "\"avg\": \"" << simplifier->avg->toString() << "\" "
                         << "}" << endl;
                    break;

                case 'd':

                    // Change simplifycation
                    TTY std::cout << "> Simplify grid to points [x] meters apart: ";
                    std::cin >> grid_s;
                    std::cout << endl;
                    grid = NULL;
                    break;
            }

            // Manual
            TTY std::cout << "> What do you want to do?" << endl <<
                    "> - [d] change grid density by simplifying (currently " << grid_s << "m)" << endl <<
                    "> - [c] change file" << endl <<
                    "> - [s] write statistics" << endl <<
                    "> - [r] run cable router" << endl <<
                    "> Do: ";

        } while(std::cin >> action);

    } while(!std::cin.eof());

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