
#ifndef _INDICOMPR_H_
#define _INDICOMPR_H_

#include "Physics/IndicoCollisionObject.h"

namespace Indico
{
	namespace Physics
	{
		class CollisionWorld;

		class MPR
		{
		private:
			CollisionWorld* mWorld;
			int mMaxIterations;
			float mTolerance;

		public:
			MPR(CollisionWorld* world, int maxIterations, float tolerance);

			bool collide(CollisionObject* a, CollisionObject* b);
		};
	}
}

#endif
