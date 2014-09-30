#include <iostream>
#include "structures.h"
#include "reader.h"
#include "simplifier.h"

#define DEFAULT_GRID_SIZE 5;

using namespace std;

int main(int argc, char const *argv[]) {
    // Start timer
    clock_t start = clock();

    int grid_s = argc >= 3 ? atoi(argv[2]) : DEFAULT_GRID_SIZE;

    Reader *r = new Reader(argc >= 2 ? argv[1] : "/Projects/Combinatorial/data/matrix 2.txt");
    point *row = new point();
    Simplifier *simplifier = new Simplifier();

    while (r->next(row)) {
        simplifier->add(row);
        row = new point();
    }

    cout << "Read " << simplifier->count << " points\n";
    cout << "Min:\n\t" << simplifier->min->toString() << "\n";
    cout << "Max:\n\t" << simplifier->max->toString() << "\n";
    cout << "Avg:\n\t" << simplifier->avg->toString() << "\n";

    // End timer
    clock_t stop = clock();
    cout << double(stop - start) / CLOCKS_PER_SEC << " seconds for input\n";

    vector<vector<cell>> grid = simplifier->grid(grid_s, grid_s);

    cout << "Generated grid of " << grid.size() << "x" << grid.begin()->size() << "\n";

//    std::vector<vector<cell>>::const_iterator i;
//    std::vector<cell>::const_iterator j;
//    int mapped = 0;
//    for(i=grid.begin(); i!=grid.end(); ++i){
//        for(j=i->begin(); j!=i->end(); ++j){
//            if(j->pipeline)
//                cout << "|";
//            else if (j->bomb)
//                cout << "b";
//            else if (j->builder)
//                cout << ".";
//            else if(j->mapped)
//                cout << " ";
//            else
//                cout << "*";
//            if(j->mapped) mapped++;
//        }
//        cout << "\n";
//    }
//    cout << (double)(mapped) / grid.size() / grid.begin()->size() * 100 << "% of grid filled\n";

    clock_t stop2 = clock();
    cout << double(stop2 - stop) / CLOCKS_PER_SEC << " seconds for grid\n";

    return 0;
}