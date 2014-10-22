#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class AngleAwareThetaStarSolver : public Solver {

        int noPoints;

    public:
        AngleAwareThetaStarSolver(int noPoints);
        void solve(Grid *grid, vector<coordinate> &line, double &time);
    };
}
