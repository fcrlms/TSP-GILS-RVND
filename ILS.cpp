#include <cmath> // INFINITY

#include "ILS.hpp"
#include "construction.hpp"
#include "localSearch.hpp"
#include "perturb.hpp"
#include "solution.hpp"

/**
 * Iterated Local Search
 */
Solution ILS (double **matrixAdj, int dimension, int maxIter, int maxIterILS)
{
	Solution bestOfAll;
	bestOfAll.cost = INFINITY;

	for (int i = 0; i < maxIter; ++i) {
		Solution s = construction(matrixAdj, dimension);

		Solution best = s;

		int iterILS = 0;

		while (iterILS <= maxIterILS) {
			localSearch(matrixAdj, &s);

			if (s.cost < best.cost) {
				best = s;
				iterILS = 0;
			}

			s = perturb(matrixAdj, &best);

			iterILS += 1;
		}

		if (best.cost < bestOfAll.cost) {
			bestOfAll = best;
		}
	}

	return bestOfAll;
}
