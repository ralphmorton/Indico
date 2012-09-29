
#ifndef _INDICOAABB_H_
#define _INDICOAABB_H_

#include "IndicoVector3.h"

namespace Indico
{
	class AABB
	{
	public:
		Vector3 mMin, mMax;

	public:
		AABB() : mMin(Vector3::ZERO), mMax(Vector3::ZERO) { }
		AABB(const Vector3& min, const Vector3& max) : mMin(min), mMax(max) { }

		bool intersects(const AABB &other) const
		{
			if (
				other.mMax.x < mMin.x ||
				other.mMax.y < mMin.y ||
				other.mMax.z < mMin.z ||
				mMax.x < other.mMin.x ||
				mMax.y < other.mMin.y ||
				mMax.z < other.mMin.z)
				return false;

			return true;
		}
	};
}

#endif
