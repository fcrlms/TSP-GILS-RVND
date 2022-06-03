#include <cstdlib> // rand()
#include <vector>
#include <cmath> // ceil()

#include "perturb.hpp"
#include "solution.hpp"

int randomNumber(int lowerLimit, int upperLimit)
{
	if (lowerLimit == upperLimit)
		return lowerLimit;
	else
		return ( std::rand() % (upperLimit - lowerLimit) ) + lowerLimit;
}

double calcDoubleBridgeCost (double **m, std::vector<int>& s, int fPos, int fSize, int sPos, int sSize)
{
	double delta = 0;

	// the segments are adjacent
	if (fPos + fSize == sPos) {
		int node_fpos = s[fPos];
		int prev_fpos = s[fPos -1];
		int prev_fend = s[fPos + fSize -1];

		int node_spos = s[sPos];
		int node_send = s[sPos + sSize];
		int prev_send = s[sPos + sSize -1];

		double oldCost = m[prev_fpos][node_fpos] + m[prev_fend][node_spos] + m[prev_send][node_send];
		double newCost = m[prev_fpos][node_spos] + m[prev_send][node_fpos] + m[prev_fend][node_send];

		delta = newCost - oldCost;
	}
	// segments not adjacent
	else {
		int node_fpos = s[fPos];
		int prev_fpos = s[fPos -1];

		int node_fend = s[fPos + fSize];
		int prev_fend = s[fPos + fSize -1];

		int node_spos = s[sPos];
		int prev_spos = s[sPos -1];

		int node_send = s[sPos + sSize];
		int prev_send = s[sPos + sSize -1];

		double oldFCost = m[prev_fpos][node_fpos] + m[prev_fend][node_fend];
		double oldSCost = m[prev_spos][node_spos] + m[prev_send][node_send];

		double newFCost = m[prev_spos][node_fpos] + m[prev_fend][node_send];
		double newSCost = m[prev_fpos][node_spos] + m[prev_send][node_fend];

		delta = (newSCost + newFCost) - (oldSCost + oldFCost);
	}

	return delta;
}

/**
 * Implements double bridge movement
 */
Solution perturb (double **m, Solution *best)
{
	Solution s = *best;

	int dimension = best->sequence.size();
	int range = dimension -1;

	int upperLimit = std::ceil(dimension / 10.);
	int lowerLimit = 2;

	int firstSegSize = randomNumber(lowerLimit, upperLimit);
	int secondSegSize = randomNumber(lowerLimit, upperLimit);

	int firstPos = randomNumber(1, range -firstSegSize);
	int secondPos = randomNumber(1, range -secondSegSize);

	/**
	 * Regenerating secondPos until it is valid. it will be valid only
	 * when the two chosen segments have no intersection
	 */
	while (
		(secondPos <= firstPos && secondPos + secondSegSize > firstPos) ||
		(firstPos <= secondPos && firstPos + firstSegSize > secondPos)
	) {
		secondPos = randomNumber(1, range -secondSegSize);
	}

	// ensures firstPos comes first and secondPos comes after
	if (secondPos < firstPos) {
		// swapping positions
		int aux = firstPos;
		firstPos = secondPos;
		secondPos = aux;

		// swapping sizes
		aux = firstSegSize;
		firstSegSize = secondSegSize;
		secondSegSize = aux;
	}

	double delta = calcDoubleBridgeCost(m, best->sequence, firstPos, firstSegSize, secondPos, secondSegSize);
	s.cost += delta;

	std::vector<int> firstSeg (firstSegSize);
	for (int i = 0; i < firstSegSize; ++i) {
		firstSeg[i] = s.sequence[firstPos +i];
	}

	std::vector<int> secondSeg (secondSegSize);
	for (int i = 0; i < secondSegSize; ++i) {
		secondSeg[i] = s.sequence[secondPos +i];
	}

	int smallestSequence = std::min(firstSegSize, secondSegSize);

	for (int i = 0; i < smallestSequence; ++i) {
		std::swap(s.sequence[firstPos +i], s.sequence[secondPos +i]);
	}

	int diff = firstSegSize - secondSegSize;

	// if the first sequence is bigger
	if (diff > 0) {
		diff = std::abs(diff);

		// toMove stores the elements that haven't been moved from the first sequence
		std::vector<int> toMove (diff);
		int offset = firstPos + secondSegSize;
		for (int i = 0; i < diff; ++i) {
			toMove[i] = s.sequence[i + offset];
		}

		// moving all elements diff positions backwards to give room for toMove
		int upper = secondPos + secondSegSize;
		for (int i = firstPos + firstSegSize; i < upper; ++i) {
			s.sequence[i - diff] = s.sequence[i];
		}

		// inserting toMove
		int insert_offset = secondPos + secondSegSize - diff;
		for (int i = 0; i < diff; i++) {
			s.sequence[insert_offset + i] = toMove[i];
		}
	}
	// if the second sequence is bigger
	else if (diff < 0) {
		diff = std::abs(diff);

		std::vector<int> toMove (diff);
		int offset = secondPos + firstSegSize;
		for (int i = 0; i < diff; ++i) {
			toMove[i] = s.sequence[i + offset];
		}

		int lower = firstPos + firstSegSize;
		for (int i = offset -1; i >= lower; --i) {
			s.sequence[i + diff] = s.sequence[i];
		}

		int insert_offset = firstPos + firstSegSize;
		for (int i = 0; i < diff; ++i) {
			s.sequence[insert_offset + i] = toMove[i];
		}
	}

	return s;
}
