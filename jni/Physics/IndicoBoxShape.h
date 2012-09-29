
#ifndef _INDICOBOXSHAPE_H_
#define _INDICOBOXSHAPE_H_

#include "Physics/IndicoCollisionShape.h"

namespace Indico
{
	namespace Physics
	{
		class BoxShape : public CollisionShape
		{
		public:
			Matrix createInertiaTensor(const Vector3& scale, float mass) const;
			Vector3 supportMap(const Vector3& dir) const;
		};
	}
}

#endif
