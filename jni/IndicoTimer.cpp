
#include "IndicoTimer.h"

#include <time.h>

namespace Indico
{
	Timer::Timer() { reset(); }

	void Timer::reset()
	{
		timespec now;
		::clock_gettime(CLOCK_REALTIME, &now);

		mStart = ((unsigned long long)now.tv_sec * 1000000000L) + now.tv_nsec;
	}

	unsigned long long Timer::nanos() const
	{
		timespec now;
		::clock_gettime(CLOCK_REALTIME, &now);

		unsigned long long nanos = ((unsigned long long)now.tv_sec * 1000000000L) + now.tv_nsec;

		return nanos - mStart;
	}
}
