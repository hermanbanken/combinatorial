#include "../solver.h"
#include "../structures.h"
#include "sampler.h"

namespace Solvers {
    class DijkstraSolver : public Solver {
    int noPoints;
    Sampler* sampler;

    public:
        DijkstraSolver(int noPoints);
        void preprocess(Grid *grid, double &time);
        void solve(Grid *grid, vector<coordinate> &line, double &time);
    };
}
