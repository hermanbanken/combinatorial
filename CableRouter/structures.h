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

const double ALLOWED_ANGLE = 8 / 180 * M_PI;
const double COST_ANGLE = 100;
const double COST_ANGLE_POW = 2;
const double COST_CABLE = 1;

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
    void project(float x, float y, float &out_x, float &out_y)const;
    void projectX(float x, float &out_x)const;
    void projectY(float y, float &out_y)const;
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
    const cell& getCell(float x, float y, Projection &p);

    int distanceToMap(unsigned long x, unsigned long y);

    float maxX(Projection &p) {
        float max;
        p.projectX(maxX(), max);
        return max;
    }
    float maxY(Projection &p) {
        float max;
        p.projectY(maxY(), max);
        return max;
    }
    float minX(Projection &p) {
        float min;
        p.projectX(minX(), min);
        return min;
    }
    float minY(Projection &p) {
        float min;
        p.projectY(minY(), min);
        return min;
    }

    /**
    * Gives the real angle in the input space
    */
    float angle(float ax, float ay, float bx, float by, const Projection &p);
    /**
     * Gives the real distance in the input space
     */
    double distance(float ax, float ay, float bx, float by, const Projection &p);
    double cost(float ax, float ay, float bx, float by, const Projection &p);
    double cost(float angle);

    /**
    * Plot ASCII art to an output stream
    */
    void plot(ostream& stream);

    void summary(ostream& stream);

    void write(string filename);
    static Grid read(string filename);

    vector<pair<unsigned long, unsigned long>> edgeNodes();
};

#endif /* defined(___cablerouter_structures___) */