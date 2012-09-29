
#ifndef _INDICOCOLLISIONSOLVER_H_
#define _INDICOCOLLISIONSOLVER_H_

#include "IndicoVector.h"
#include "Physics/IndicoContactManifold.h"

namespace Indico
{
	namespace Physics
	{
		class Simulator;

		class CollisionSolver
		{
		private:
			Simulator* mSimulator;

		public:
			CollisionSolver();

			void prep(float dt, const Vector<ContactManifold*>& manifolds);
			void step(const Vector<ContactManifold*>& manifolds);
		};
	}
}

#endif
