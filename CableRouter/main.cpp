#include <iostream>
#include "structures.h"
#include "reader.h"
#include "simplifier.h"

using namespace std;

int main(int argc, char const *argv[]) {
    // Start timer
    clock_t start = clock();

    Reader *r = new Reader(argc >= 2 ? argv[1] : "/Projects/Combinatorial/data/matrix 2.txt");
    point *row = new point();
    int c = 0;
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
    cout << double(stop - start) / CLOCKS_PER_SEC << " seconds\n";
    return 0;
}