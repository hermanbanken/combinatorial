#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class AStarSolver : public Solver {

    int noPoints;

    public:
        AStarSolver(int noPoints);
        void solve(Grid *grid, vector<coordinate> &line, double &time);
    };
}
