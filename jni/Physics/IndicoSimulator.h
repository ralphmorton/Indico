
#ifndef _INDICOSIMULATOR_H_
#define _INDICOSIMULATOR_H_

#include "IndicoSingleton.h"
#include "IndicoVector.h"
#include "IndicoVector3.h"
#include "Physics/IndicoCollisionSolver.h"
#include "Physics/IndicoCollisionWorld.h"

namespace Indico
{
	namespace Physics
	{
		class Simulator : public Singleton<Simulator>
		{
		protected:
			friend class CollisionObject;
			friend class RigidBody;

		private:
			Vector3 mGravity;
			float mMaxContactSeparation;
			float mAllowedPenetration;
			float mCollisionBiasFactor;
			float mRestingVelocityLimit;
			unsigned int mIterations;

			CollisionWorld mCollisionWorld;
			CollisionSolver mCollisionSolver;

			Vector<RigidBody*> mBodies;

		public:
			Simulator(Broadphase& broadphase);
			~Simulator();

			void addBody(RigidBody* body);
			void addCollisionObject(CollisionObject* obj);

			void removeBody(RigidBody* body);
			void removeCollisionObject(CollisionObject* obj);

			void step(float dt);

			void clear();

			inline Vector3 getGravity() const { return mGravity; }
			inline float getMaxContactSeparation() const { return mMaxContactSeparation; }
			inline float getAllowedPenetration() const { return mAllowedPenetration; }
			inline float getCollisionBiasFactor() const { return mCollisionBiasFactor; }
			inline float getRestingVelocityLimit() const { return mRestingVelocityLimit; }
			inline uint getIterations() const { return mIterations; }

			inline void setGravity(const Vector3 &gravity) { mGravity = gravity; }
			inline void setMaxContactSeparation(float val) { mMaxContactSeparation = val; }
			inline void setAllowedPenetration(float val) { mAllowedPenetration = val; }
			inline void setCollisionBiasFactor(float val) { mCollisionBiasFactor = val; }
			inline void setRestingVelocityLimit(float val) { mRestingVelocityLimit = val; }
			inline void setIterations(uint val) { mIterations = val; }
		};
	}
}

#endif
