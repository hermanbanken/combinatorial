#include "../solver.h"
#include "../structures.h"
#include "sampler.h"

namespace Solvers {
    class ThetaStarSolver : public Solver {

        int noPoints;
        Sampler* sampler;

    public:
        ThetaStarSolver(int noPoints);

        void preprocess(Grid *grid, double &time);
        void solve(Grid *grid, vector<coordinate> &line, double &time);
    };
}
