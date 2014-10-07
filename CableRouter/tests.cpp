#include <iostream>
#include "structures.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

bool testMapDistance(){
    vector<vector<cell>> r = vector<vector<cell>>(3, vector<cell>(3, cell()));
    r[0][0] = cell(); r[0][0].mapped = false ;
    r[1][0] = cell(); r[1][0].mapped = false ;
    r[2][0] = cell(); r[2][0].mapped = true  ;
    r[0][1] = cell(); r[0][1].mapped = false ;
    r[1][1] = cell(); r[1][1].mapped = false ;
    r[2][1] = cell(); r[2][1].mapped = true  ;
    r[0][2] = cell(); r[0][2].mapped = false ;
    r[1][2] = cell(); r[1][2].mapped = false ;
    r[2][2] = cell(); r[2][2].mapped = true  ;
    cout << "TEST map distance: ";

    Grid g = Grid(r, *Projection::identity());
    vector<pair<unsigned long, unsigned long>> edges = g.edgeNodes();

    for(vector<pair<unsigned long, unsigned long>>::const_iterator i = edges.begin(); i < edges.end(); i++){
        cout << "edge(" << i->first << "," << i->second << ")" << endl;
        g.data()->at(i->first)[i->second].distanceToMap = 3;
    }

    g.plot(cout);
    cout << "OK if contains a center node with a different mark (3)." << endl;
    return true;
}