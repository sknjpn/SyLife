#include "Random.h"

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

boost::random::random_device g_rndDev;
boost::random::mt19937 g_rndGen(g_rndDev());

double Random(double min, double max)
{
	return boost::random::uniform_real_distribution<double>(min, max)(g_rndGen);
}

bool RandomBool(double a)
{
	return Random(0.0, 1.0) <= a;
}

int Random(int min, int max)
{
	return boost::random::uniform_real_distribution<int>(min, max)(g_rndGen);
}
