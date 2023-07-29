#include <iostream>
#include <cstdlib> // srand()
#include <ctime> // time()
#include <chrono> // measuring time

#include "readData.h"
#include "ILS.h"
#include "solution.h"

#define TESTS_TO_RUN 10

typedef struct runInfo {
	double cost;
	std::chrono::microseconds duration;
} RunInfo;

int main (int argc, char **argv)
{
	std::srand(std::time(0));

	double **matrixAdj; // adjacency matrix
	int dimension; // number of nodes in the graph

	readData(argc, argv, &dimension, &matrixAdj);

	int maxIter = 50;
	int maxIterILS = ( dimension >= 150 ) ? dimension/2 : dimension;

	RunInfo testData[TESTS_TO_RUN];

	for (int i = 0; i < TESTS_TO_RUN; ++i) {
		auto start = std::chrono::high_resolution_clock::now();

		Solution s = ILS(matrixAdj, dimension, maxIter, maxIterILS);

		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		testData[i].cost = s.cost;
		testData[i].duration = duration;

		std::cout << "finished test [" << i+1 << "]\n";
	}

	std::cout << "\n";

	double meanCost = 0;
	double meanDuration = 0;

	for (int i = 0; i < TESTS_TO_RUN; ++i) {
		std::cout << "Display for test no. " << i+1 << "\n";
		std::cout << "cost: " << testData[i].cost << "\n";
		std::cout << "duration: " << testData[i].duration.count() / (double) 1000000 << " seconds\n\n";

		meanCost += testData[i].cost;
		meanDuration += testData[i].duration.count();
	}

	meanCost /= TESTS_TO_RUN;
	meanDuration /= TESTS_TO_RUN;

	double meanDurationSec = meanDuration / 1000000;

	std::cout << "mean cost: " << meanCost << "\n";
	std::cout << "mean duration: " << meanDurationSec << " seconds\n";\

	exit(EXIT_SUCCESS);
}
