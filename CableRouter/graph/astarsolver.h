#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class AStarSolver : public Solver {

    public:
        void solve(Grid *grid, vector<coordinate> &line);
    };
}
