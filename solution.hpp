#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

typedef struct solution {
	std::vector<int> sequence;
	double cost;
} Solution;

void printSolution (Solution *s);
void updateSolutionCost (Solution *s, double **costMatrix);

#endif
