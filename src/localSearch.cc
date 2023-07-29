#include <cstdlib> // rand()
#include <vector> // swap()
#include <cmath> // ceil()

#include "localSearch.h"
#include "solution.h"

#define NL_SIZE 5

bool bestImprovementSwap (double **matrixAdj, Solution *s);
bool bestImprovement2Opt (double **matrixAdj, Solution *s);
bool bestImprovementOrOpt (double **matrixAdj, Solution *s, int n);

/**
 * Based on RVND (Random Variable Neighborhood Descent)
 */
void localSearch (double **matrixAdj, Solution *s)
{
	int NL[NL_SIZE] = { 1, 2, 3, 4, 5 };
	int offset = 0;

	while (offset < NL_SIZE -1) {
		bool improved = false;

		int randomIndex = std::rand() % (NL_SIZE - offset);
		int n = NL[randomIndex];

		switch (n) {
		case 1:
			improved = bestImprovementSwap(matrixAdj, s);
			break;
		case 2:
			improved = bestImprovement2Opt(matrixAdj, s);
			break;
		case 3:
			improved = bestImprovementOrOpt(matrixAdj, s, 1); // Reinsertion
			break;
		case 4:
			improved = bestImprovementOrOpt(matrixAdj, s, 2); // Or-opt2
			break;
		case 5:
			improved = bestImprovementOrOpt(matrixAdj, s, 3); // Or-opt3
			break;
		}

		if (improved) {
			// reset
			offset = 0;
		} else {
			// remove choice
			std::swap(NL[randomIndex], NL[NL_SIZE -1 -offset]);
			offset += 1;
		}
	}

	return;
}

double calculateSwapCost (double **m, std::vector<int>& s, int i, int j)
{
	double delta = 0;

	int prev_i = s[i-1];
	int curr_i = s[i];
	int next_i = s[i+1];

	int prev_j = s[j-1];
	int curr_j = s[j];
	int next_j = s[j+1];

	// if the nodes are adjacents
	if (i +1 == j) {
		double oldCost_i = m[prev_i][curr_i];
		double oldCost_j = m[curr_j][next_j];

		double newCost_i = m[curr_i][next_j];
		double newCost_j = m[prev_i][curr_j];

		delta = (newCost_i + newCost_j) - (oldCost_i + oldCost_j);
	} else {
		// cost{i-1, i} + cost{i, i+1}
		double oldCost_i = m[prev_i][curr_i] + m[curr_i][next_i];

		// cost{j-1, j} + cost{j, j+1}
		double oldCost_j = m[prev_j][curr_j] + m[curr_j][next_j];

		// cost{i-1, j} + cost{j, i+1}
		double newCost_i = m[prev_j][curr_i] + m[curr_i][next_j];

		// cost{j-1, i} + cost{i, j+1}
		double newCost_j = m[prev_i][curr_j] + m[curr_j][next_i];

		delta = (newCost_i + newCost_j) - (oldCost_i + oldCost_j);
	}

	return delta;
}

bool bestImprovementSwap (double **matrixAdj, Solution *s)
{
	double bestDelta = 0;
	double best_i = 0;
	double best_j = 0;

	int range = s->sequence.size() -1;

	for (int i = 1; i < range -1; ++i)
	for (int j = i +1; j < range; ++j) {
		double thisDelta = calculateSwapCost(matrixAdj, s->sequence, i, j);

		if (thisDelta < bestDelta) {
			bestDelta = thisDelta;
			best_i = i;
			best_j = j;
		}
	}

	if (bestDelta < 0) {
		std::swap(s->sequence[best_i], s->sequence[best_j]);

		s->cost += bestDelta;

		return true;
	}

	return false;
}

double calculate2OptCost (double **m, std::vector<int>& s, int i, int j)
{
	double delta = 0;

	int node_i = s[i];
	int next_i = s[i +1];

	int node_j = s[j];
	int next_j = s[j +1];

	double oldCost = m[node_i][next_i] + m[node_j][next_j];
	double newCost = m[node_i][node_j] + m[next_i][next_j];

	delta = newCost - oldCost;

	return delta;
}

void exec2Opt (std::vector<int>& s, int i, int j)
{
	// node i+1 to node j
	int range = std::ceil((j - i) / 2);

	// inverts the segment between the edges {i, i+1} and {j, j+1}
	for (int n = 0; n < range; ++n) {
		int node_i = i+1 + n; // i+1 is the initial node
		int node_j = j -n; // j is the final node

		std::swap(s[node_i], s[node_j]);
	}
}

bool bestImprovement2Opt (double **matrixAdj, Solution *s)
{
	double bestDelta = 0;

	double best_i = 0; // ith edge
	double best_j = 0; // jth edge

	int range = s->sequence.size() -1;

	/**
	 * When i=0 we have a special case where the final edge can't be chosen
	 * because it is adjacent to the first edge since it is a loop
	 */
	for (int j = 2; j < range; ++j) {
		double thisDelta = calculate2OptCost(matrixAdj, s->sequence, 0, j);

		if (thisDelta < bestDelta) {
			bestDelta = thisDelta;
			best_i = 0;
			best_j = j;
		}
	}

	// remaining cases
	for (int i = 1; i < range -2; ++i)
	for (int j = i +2; j < range; ++j) {
		double thisDelta = calculate2OptCost(matrixAdj, s->sequence, i, j);

		if (thisDelta < bestDelta) {
			bestDelta = thisDelta;
			best_i = i;
			best_j = j;
		}
	}

	if (bestDelta < 0) {
		exec2Opt(s->sequence, best_i, best_j);

		s->cost += bestDelta;

		return true;
	}

	return false;
}

void execOrOpt(std::vector<int>& s, int best_start, int n, int best_posInsert)
{
	// sequence that will be repositioned
	int chain[3];

	for (int i = 0; i < n; ++i) {
		chain[i] = s[best_start + i];
	}

	bool forward = (best_posInsert >= best_start);

	if (forward) {
		// moving elements ahead of the chain n positions backwards
		for (int i = best_start + n; i <= best_posInsert; ++i) {
			s[i - n] = s[i];
		}

		/**
		 * The insertion point has moved n positions backwards
		 * so we begin at (best_posInsert - n + 1)
		 */
		for (int i = 0; i < n; ++i) {
			s[(best_posInsert - n +1) + i] = chain[i];
		}
	} else {
		/**
		 * Moving elements behind the chain (until the insertion point)
		 * n positions forward
		 */
		for (int i = best_start -1; i > best_posInsert; --i) {
			s[i + n] = s[i];
		}

		/**
		 * The insertion point hasn't moved so we begin inserting at
		 * (best_posInsert +1)
		 */
		for (int i = 0; i < n; ++i) {
			s[(best_posInsert +1) + i] = chain[i];
		}
	}
}

double calculateOrOptCost (double **m, std::vector<int>& s, int start, int end, int pos)
{
	double delta = 0;

	int node_start = s[start];
	int prev_start = s[start -1];

	int node_end = s[end];
	int prev_end = s[end -1];

	int node_insert = s[pos];
	int next_insert = s[pos +1];

	double oldCost = m[prev_start][node_start] + m[prev_end][node_end] + m[node_insert][next_insert];
	double newCost = m[prev_start][node_end] + m[prev_end][next_insert] + m[node_insert][node_start];

	delta = newCost - oldCost;

	return delta;
}

bool bestImprovementOrOpt (double **matrixAdj, Solution *s, int n)
{
	double bestDelta = 0;
	double best_start = 0;
	double best_posInsert = 0;

	int range = s->sequence.size() -n;

	/**
	 * The first element of the chain is start
	 * The last element of the chain is end-1
	 */
	for (int start = 1; start < range; ++start) {
		int end = start + n;

		// i is the position to insert, the first node will be put at i+1
		for (int i = 0; i < start -1; ++i) {
			double thisDelta = calculateOrOptCost(matrixAdj, s->sequence, start, end, i);

			if (thisDelta < bestDelta) {
				bestDelta = thisDelta;
				best_start = start;
				best_posInsert = i;
			}
		}

		for (int i = end; i < range; ++i) {
			double thisDelta = calculateOrOptCost(matrixAdj, s->sequence, start, end, i);

			if (thisDelta < bestDelta) {
				bestDelta = thisDelta;
				best_start = start;
				best_posInsert = i;
			}
		}
	}

	if (bestDelta < 0) {
		execOrOpt(s->sequence, best_start, n, best_posInsert);

		s->cost += bestDelta;
		return true;
	}

	return false;
}
