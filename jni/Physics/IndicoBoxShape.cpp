
#include "Physics/IndicoBoxShape.h"

namespace Indico
{
	namespace Physics
	{
		Matrix BoxShape::createInertiaTensor(const Vector3& scale, float mass) const
		{
			Matrix it = Matrix::IDENTITY;

			it.m[0][0] = (1.0f / 12.0f) * mass * (scale.y * scale.y + scale.z * scale.z);
			it.m[1][1] = (1.0f / 12.0f) * mass * (scale.x * scale.x + scale.z * scale.z);
			it.m[2][2] = (1.0f / 12.0f) * mass * (scale.x * scale.x + scale.y * scale.y);

			return it;
		}

		Vector3 BoxShape::supportMap(const Vector3& dir) const
		{
			Vector3 support = Vector3::ZERO;
			support.x = (dir.x > 0.f) ? 0.5f : -0.5f;
			support.y = (dir.y > 0.f) ? 0.5f : -0.5f;
			support.z = (dir.z > 0.f) ? 0.5f : -0.5f;
			return support;
		}
	}
}
