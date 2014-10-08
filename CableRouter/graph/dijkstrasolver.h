#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class DijkstraSolver : public Solver {

    public:
        void solve(Grid *grid, vector<coord> &line);
    };
}