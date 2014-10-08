#include <stdio.h>
#include <float.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <stdexcept>
#include <iostream>

#ifndef ___cablerouter_structures___
#define ___cablerouter_structures___

#define coord pair<unsigned long,unsigned long>

const double ALLOWED_ANGLE = 8.0 / 180.0 * M_PI;
const double COST_ANGLE = 100;
const double COST_ANGLE_POW = 2;
const double COST_CABLE = 1;
const double COST_OFFMAP = 100;

using namespace std;

enum SeaBed : char {
    Sand, Rock, Mud
};

struct cell {
    bool mapped = false;
    double z;
    bool builder;
    bool bomb;
    bool pipeline;
    SeaBed seabed;
    int distanceToMap = -1;
    int distanceExplored = 0;
};

struct point {
    point(float x, float y, float z);

    point();

    float x;
    float y;
    float z;
    SeaBed seabed;
    bool builder;
    bool bomb;
    bool pipeline;

    string toString();
};

struct aggregationCell {
    int count = 0;
    double z = 0;
    bool builder;
    bool bomb;
    bool pipeline;
    int beds[3]; // SeaBed.count

    void addPoint(point *p);

    void addBed(SeaBed b);

    cell toCell();

    SeaBed seabed();

};

/**
* Converts input points using its parameters.
* P(A) = (A+offset)*scale = B
*/
class Projection {
private:
    bool is_identity = false;
    bool is_equalScale = false;
public:
    const float offset_x;
    const float offset_y;
    const float scale_x;
    const float scale_y;
    Projection(float offset_x, float offset_y, float scale_x, float scale_y);
    void project(double x, double y, double &out_x, double &out_y)const;
    void projectX(double x, double &out_x)const;
    void projectY(double y, double &out_y)const;
    Projection back();
    bool isIdentity()const {
        return is_identity;
    }
    bool hasEqualScales()const {
        return is_equalScale;
    }
    static Projection identity() {
        return Projection(0, 0, 1, 1);
    }


};

class Grid {
private:
    vector<vector<cell>> grid;
    Projection gridProjection;

    float maxX() {
        return grid.size();
    }
    float maxY() {
        return grid.at(0).size();
    }
    float minX() {
        return 0;
    }
    float minY() {
        return 0;
    }

    cell& get(unsigned long x, unsigned long y);
    bool tryGet(unsigned long x, unsigned long y, cell &out_cell);

    int distanceToMap(unsigned long x, unsigned long y, int maxDistance);

    void floodFindDistancesToEdge();

public:
    /**
    * Returns pointer to internal data
    */
    vector<vector<cell>>& data(){
        return this->grid;
    }

    /**
    * Creates grid, copying the data
    */
    Grid(vector<vector<cell>> grid, Projection fromInputToGrid);
    Projection backToInputProjection();
    const Projection& inputProjection();
    Projection to_ZeroToOne_Projection();
    const cell& getCell(double x, double y, Projection &p);

    int distanceToMap(unsigned long x, unsigned long y);

    double maxX(Projection &p) {
        double max;
        p.projectX(maxX(), max);
        return max;
    }
    double maxY(Projection &p) {
        double max;
        p.projectY(maxY(), max);
        return max;
    }
    double minX(Projection &p) {
        double min;
        p.projectX(minX(), min);
        return min;
    }
    double minY(Projection &p) {
        double min;
        p.projectY(minY(), min);
        return min;
    }

    /**
    * Gives the real angle in the input space
    */
    double angle(double ax, double ay, double bx, double by, const Projection &p);
    double angle(double angle1, double angle2);
    /**
     * Gives the real distance (in meters) in the input space
     */
    double distance(double ax, double ay, double bx, double by, const Projection &p);
    double cost(double ax, double ay, double bx, double by, const Projection &p, bool gradient = false);
    double cost(double angle, bool gradient = false);

    /**
    * Plot ASCII art to an output stream
    */
    void plot(ostream& stream);

    void summary(ostream& stream);

    void write(string filename);
    static Grid read(string filename);

    vector<pair<cell,coord>> edgeNodes();
};

#endif /* defined(___cablerouter_structures___) */