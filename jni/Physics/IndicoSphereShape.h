
#ifndef _INDICOSPHERESHAPE_H_
#define _INDICOSPHERESHAPE_H_

#include "Physics/IndicoCollisionShape.h"

namespace Indico
{
	namespace Physics
	{
		class SphereShape : public CollisionShape
		{
		public:
			Matrix createInertiaTensor(const Vector3& scale, float mass) const;
			Vector3 supportMap(const Vector3& dir) const;
		};
	}
}

#endif
