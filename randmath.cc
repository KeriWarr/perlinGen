#include "randmath.h"

void randMath::seedRand() {
	
	static bool seeded = false;
	
	if(!seeded)	{
		
		srand(time(NULL));
		seeded = true;
		
	}	
	
}

int randMath::getRand(const int bot, const int top) {
	
	return (rand() % (top - bot + 1)) + bot;
	
}
