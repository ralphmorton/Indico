
#include "Physics/IndicoSphereShape.h"

namespace Indico
{
	namespace Physics
	{
		Matrix SphereShape::createInertiaTensor(const Vector3& scale, float mass) const
		{
			Matrix it = Matrix::IDENTITY;

			it.m[0][0] = 0.4f * (scale.x * scale.x) * mass;
			it.m[1][1] = 0.4f * (scale.y * scale.y) * mass;
			it.m[2][2] = 0.4f * (scale.z * scale.z) * mass;

			return it;
		}

		Vector3 SphereShape::supportMap(const Vector3& dir) const
		{
			Vector3 d = dir;
			d.normalise();

			return d * 0.5f;
		}
	}
}
