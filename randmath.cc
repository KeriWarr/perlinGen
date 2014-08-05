#include "randmath.h"

void randMath::seedRand() {
	
	static bool seeded = false;
	
	if(!seeded)	{
		
		srand(time(NULL));
		seeded = true;
		
	}	
	
}

int randMath::getRand(const int bot, const int top) {
	
	randMath::seedRand();
	
	return (rand() % (top - bot + 1)) + bot;
	
}

float randMath::getRand(const float bot, const float top) {
	
	randMath::seedRand();
	
	return (((float)((rand()%((int)((top-bot+1)*1000)))+(int)bot))/1000.0);
	
}
