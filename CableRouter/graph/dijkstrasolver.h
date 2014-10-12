#include "../solver.h"
#include "../structures.h"

namespace Solvers {
    class DijkstraSolver : public Solver {
    int noPoints;

    public:
        DijkstraSolver(int noPoints);
        void solve(Grid *grid, vector<coordinate> &line);
    };
}
