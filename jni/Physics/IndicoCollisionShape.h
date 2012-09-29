
#ifndef _INDICOCOLLISIONSHAPE_H_
#define _INDICOCOLLISIONSHAPE_H_

#include "IndicoMatrix.h"
#include "IndicoVector3.h"

namespace Indico
{
	namespace Physics
	{
		class CollisionShape
		{
		public:
			virtual ~CollisionShape() { }

			virtual Matrix createInertiaTensor(const Vector3& scale, float mass) const = 0;
			virtual Vector3 supportMap(const Vector3& dir) const = 0;
		};
	}
}

#endif
