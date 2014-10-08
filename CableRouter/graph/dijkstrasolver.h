#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class DijkstraSolver : public Solver {

    public:
        void solve(Grid *grid, vector<tuple<unsigned long, unsigned long>> &line);
    };
}