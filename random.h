
/* random.h
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <time.h>
#include <stdlib.h>

class Random
{
private:
	int seed;
public:
	Random() { randomize(); }

	void randomize() { seed = (unsigned)time(NULL); }
	void setSeed(int newSeed) { seed = newSeed; }
	unsigned getSeed() const { return seed; }

	int gen() { srand(seed); seed = rand(); return seed; }

	int next(const int x) { return gen() % x; }
	double next() { return gen() / (RAND_MAX + 1.0); }
};


#endif
