#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class ThetaStarSolver : public Solver {

        int noPoints;

    public:
        ThetaStarSolver(int noPoints);
        void solve(Grid *grid, vector<coordinate> &line);
    };
}
