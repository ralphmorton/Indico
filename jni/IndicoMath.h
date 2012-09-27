
#ifndef _INDICOMATH_H_
#define _INDICOMATH_H_

#include <math.h>

#include <assert.h>

namespace Indico
{
	class Math
	{
	public:
		static const float PI;
        static const float TWO_PI;
        static const float HALF_PI;

	public:
		template <typename T> static T clamp(T val, T minval, T maxval)
		{
			assert(minval <= maxval && "Invalid clamp range");

			T mn = val < maxval ? val : maxval;
			return mn > minval ? mn : minval;
		}

		template <typename T> static T min(T a, T b)
		{
			return a<b?a:b;
		}

		template <typename T> static T max(T a, T b)
		{
			return a>b?a:b;
		}

		static inline bool floatEqual(float a, float b, float tolerance) { return ::fabs(b-a) <= tolerance; }

		static inline bool isNaN(float f) { return f != f; }

		static inline float abs(float val) { return ::fabs(val); }
		static inline float sqrt(float val) { return ::sqrt(val); }
		static inline float pow(float val, float exp) { return ::pow(val, exp); }

		static inline float cos(float val) { return ::cos(val); }
		static inline float sin(float val) { return ::sin(val); }
		static inline float tan(float val) { return ::tan(val); }

		static inline float acos(float val) { return ::acos(val); }
		static inline float asin(float val) { return ::asin(val); }
		static inline float atan(float val) { return ::atan(val); }
	};
}

#endif
