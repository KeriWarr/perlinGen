#ifndef __RANDMATH_H__
#define __RANDMATH_H__
#include <ctime>
#include <cstdlib>
#include <cmath>


class randMath {

	public:

		// calls srand(time(NULL)); prevents multiple seedings
		static void seedRand();

		// generates an integer in between bot and top, inclusive
		static int getRand(const int bot, const int top);
	
};

#endif
