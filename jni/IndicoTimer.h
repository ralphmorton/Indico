
#ifndef _INDICOTIMER_H_
#define _INDICOTIMER_H_

namespace Indico
{
	class Timer
	{
	private:
		unsigned long long mStart;

	public:
		Timer();

		void reset();

		unsigned long long nanos() const;
	};
}

#endif
