#include <stdio.h>
#include <float.h>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>
#include <math.h>
#include <stdexcept>
#include <iostream>

#ifndef ___cablerouter_structures___
#define ___cablerouter_structures___

#define coord pair<unsigned long,unsigned long>
#define coordinate pair<double,double>

#define ABS(a) (a > 0 ? a : (-1 * a))
#define EUCL(ax,ay,bx,by) (sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)))
#define ANGL(ax,ay,bx,by) atan2(by - ay, bx - ax)

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

    inline double exactLimitToGradient(double limit, double width_of_gradient_factor, double input);

    cell& get(unsigned long x, unsigned long y);
    bool tryGet(unsigned long x, unsigned long y, cell &out_cell);

    int distanceToMap(unsigned long x, unsigned long y, int maxDistance);



public:
    void floodFindDistancesToEdge();
    /**
    * Returns pointer to internal data
    */
    vector<vector<cell>>& data(){
        return this->grid;
    }

    /**
    * Costs
    */
    double ALLOWED_ANGLE =   8.0 / 180.0 * M_PI;
    double TOO_SHORT =       50;
    double COST_ANGLE =      5000000;
    double COST_CABLE =      1;
    double COST_OFFMAP =     50000;
    double COST_TOO_SHORT =  50000;
    double COST_PIXEL_BOMB = 50000.0;
    double COST_PIXEL_PIPE = 5000000.0;
    double COST_PIXEL_ROCK = 50.0;

    /**
    * Creates grid, copying the data
    */
    Grid(vector<vector<cell>> grid, Projection fromInputToGrid);
    Projection backToInputProjection();
    const Projection& inputProjection();
    Projection to_ZeroToOne_Projection();
    const cell& getCell(double x, double y, const Projection &p);

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

    double cost(const vector<coordinate> line, const Projection &p);
    double cost(const coordinate start, const double *x, const unsigned int N, const coordinate end, const Projection &p);

    /**
    * Plot ASCII art to an output stream
    */
    void plot(ostream& stream, vector<coordinate> line = vector<coordinate>(0));

    void summary(ostream& stream);

    void write(string filename);
    static Grid read(string filename);

    vector<pair<cell,coord>> edgeNodes();

    bool lineOfSight(double x0, double y0, double x1, double y1, Projection const &p);
};

#endif /* defined(___cablerouter_structures___) */
