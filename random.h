
/* random.h
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <ctime>
#include <cstdlib>

class Random
{
private:
	int seed;
public:
	Random() { randomize(); }

	void randomize() { seed = (unsigned)time(nullptr); }
	void setSeed(const int newSeed) { seed = newSeed; }
	unsigned getSeed() const { return seed; }

	int gen() { srand(seed); seed = rand(); return seed; }

	int next(const int x) { return gen() % x; }
	double next() { return gen() / (RAND_MAX + 1.0); }
};


#endif
