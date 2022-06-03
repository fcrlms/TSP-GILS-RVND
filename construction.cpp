#include <cstdlib> // rand()
#include <vector> // swap()
#include <algorithm> // sort()
#include <cmath> // ceil()

#include "construction.hpp"
#include "solution.hpp"

// Possible values of alpha
double alphaValues[25] = {
	0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07,
	0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14,
	0.15, 0.16, 0.17, 0.18, 0.19, 0.20, 0.21,
	0.22, 0.23, 0.24, 0.25
};

typedef struct insertionInfo {
	double insertedNode; // Node k to insert
	int removedEdge; // Edge {i, j} where k will be inserted at (where i = removedEdge)
	int cost; // Cost of the insertion, negative if the cost decreases
} InsertionInfo;

void prepareConstruction (double **matrixAdj, int dimension, Solution *s, std::vector<int> *candidateList);

std::vector<InsertionInfo> calculateInsertionsCost (double **matrixAdj, Solution *s, int node);

void sortInCrescentCost (std::vector<InsertionInfo> *insertions);

void insertInSolution (Solution *s, InsertionInfo& insertion);

/**
 * Based on GRASP (Greedy Randomized Adaptive Search Procedure)
 */
Solution construction (double **matrixAdj, int dimension)
{
	Solution s;
	std::vector<int> candidateList;

	prepareConstruction(matrixAdj, dimension, &s, &candidateList);

	while (!candidateList.empty()) {
		int node_k = candidateList.back();

		std::vector<InsertionInfo> costInsertions = calculateInsertionsCost(matrixAdj, &s, node_k);

		sortInCrescentCost(&costInsertions);

		double alpha = alphaValues[std::rand() % 25];

		int randomIndex = std::rand() % (int) ( std::ceil(alpha * costInsertions.size()) );
		InsertionInfo chosenInsertion = costInsertions.at(randomIndex);

		insertInSolution(&s, chosenInsertion);

		// removes the inserted node from the candidate list
		candidateList.pop_back();
	}

	return s;
}

/**
 * Creates a subtour starting and ending with the initial node (node 1)
 * and adds three random nodes in the middle, stores them in s->sequence
 * and then calculates the cost of the subtour
 *
 * candidateList are all remaining nodes
 */
void prepareConstruction (double **matrixAdj, int dimension, Solution *s, std::vector<int> *candidateList)
{
	// fills the candidateList (without node 1)
	for (int i = 2; i <= dimension; ++i) {
		candidateList->push_back(i);
	}

	s->sequence.push_back(1);

	for (int i = 0; i < 3; ++i) {
		int upperLimit = candidateList->size();

		int randomIndex = std::rand() % upperLimit;

		s->sequence.push_back((*candidateList)[randomIndex]);

		// swaps the position of the last element with the chosen element
		std::swap((*candidateList)[randomIndex], (*candidateList)[upperLimit -1]);

		// removes chosen element from candidateList
		candidateList->pop_back();
	}

	s->sequence.push_back(1);

	updateSolutionCost(s, matrixAdj);
}

std::vector<InsertionInfo> calculateInsertionsCost (double **matrixAdj, Solution *s, int node)
{
	int possibleInsertions = s->sequence.size() -1;

	std::vector<InsertionInfo> insertions (possibleInsertions);

	for (int i = 0; i < possibleInsertions; ++i) {
		InsertionInfo currentInsertion;
		currentInsertion.insertedNode = node;
		currentInsertion.removedEdge = i;

		// node i
		int prevNode = s->sequence[i];

		// node j
		int nextNode = s->sequence[i +1];

		// cost{i, j}
		double oldEdgeCost = matrixAdj[prevNode][nextNode];

		// cost{i, k} + cost{k, j}
		double newEdgesCost = matrixAdj[prevNode][node] + matrixAdj[node][nextNode];

		currentInsertion.cost = newEdgesCost - oldEdgeCost;

		insertions.at(i) = currentInsertion;
	}

	return insertions;
}

bool compareInsertionsCosts (InsertionInfo& a, InsertionInfo& b)
{
	return a.cost < b.cost;
}

void sortInCrescentCost (std::vector<InsertionInfo> *insertions)
{
	std::sort(insertions->begin(), insertions->end(), compareInsertionsCosts);
}


void insertInSolution (Solution *s, InsertionInfo& insertion)
{
	std::vector<int>::iterator it = s->sequence.begin() +1;

	s->sequence.insert(it + insertion.removedEdge, insertion.insertedNode);

	s->cost += insertion.cost;
}
