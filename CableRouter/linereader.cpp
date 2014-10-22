#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <bitset>
#include <fstream>
#include <vector>
#include <math.h>

#define EUCL(ax,ay,bx,by) (sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)))
#define ANGL(ax,ay,bx,by) atan2(by - ay, bx - ax)

using namespace std;

int main(int argc, char const *argv[]) {
    if(argc < 2){
        cout << "Usage: \treadline <file>" << endl;
        cout << "\t\tWhere <file> is a file containing coordinates x, y, cablelength" << endl;
        exit(0);
    }
    string file = argv[1];

    ifstream in(file);
    if (!in.good()){
        cout << "The file is not readable.";
        exit(0);
    }

    double x, y, cablelength;
    vector<pair<double, double>> line;
    pair<double, double> point1, point2, newpoint;
    double dx, dy, newdx, newdy;
    while ( in >> x >> y >> cablelength )
    {
        newpoint = make_pair(x, y);
        if(line.size() < 2) {
            line.push_back(newpoint);
            continue;
        }

        point1 = line.at(line.size()-2);
        point2 = line.at(line.size()-1);

        dx = point2.first - point1.first;
        dy = point2.second - point1.second;

        newdx = newpoint.first - point2.first;
        newdy = newpoint.second - point2.second;

        if (fabs(ANGL(point1.first, point1.second, point2.first, point2.second)- ANGL(point2.first, point2.second, newpoint.first, newpoint.second)) < 0.1f)
            line.at(line.size()-1) = newpoint;
        else
            line.push_back(newpoint);
    }

    for(int i = 0; i + 1 < line.size(); i++) {
        double distance = EUCL(line[i].first,line[i].second, line[i+1].first, line[i+1].second);
        cout << "Line from (" << line[i].first << "," << line[i].second << ") to (" << line[i + 1].first << "," << line[i + 1].second << "), distance is " << distance << "m." << endl;
    }
    exit(0);
}
