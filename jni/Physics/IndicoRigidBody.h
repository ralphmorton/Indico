
#ifndef _INDICORIGIDBODY_H_
#define _INDICORIGIDBODY_H_

#include "IndicoMatrix.h"
#include "IndicoVector3.h"
#include "Physics/IndicoCollisionShape.h"

namespace Indico
{
	namespace Physics
	{
		class CollisionObject;

		class RigidBody
		{
		private:
			friend class CollisionObject;
			friend class Simulator;

		private:
			CollisionObject* mObject;
			float mRestitution;
			float mFriction;
			float mLinearDamping;
			float mAngularDamping;

			float mInverseMass;
			Matrix mInverseInertiaTensor;
			Matrix mInverseInertiaTensorWorld;
			Vector3 mForce;
			Vector3 mTorque;
			Vector3 mLinearVelocity;
			Vector3 mAngularVelocity;
			Vector3 mBiasLinearVelocity;
			Vector3 mBiasAngularVelocity;

			bool mActive;
			unsigned short mIslandID;
			float mMotion;

		public:
			RigidBody(CollisionShape& shape, const Transform& transform, float mass, float restitution, float friction, float linearDamping, float angularDamping);
			~RigidBody();

			void applyImpulse(const Vector3& impulse, const Vector3& relativePosition);
			void applyBiasImpulse(const Vector3& impulse, const Vector3& relativePosition);

			Vector3 getVelocity(const Vector3& relativePosition) const;
			Vector3 getBiasVelocity(const Vector3& relativePosition) const;

			float getImpulseDenominator(const Vector3& relativePosition, const Vector3& normal) const;

			Transform& getTransform();
			const CollisionShape& getShape();

			void update();

			inline float getRestitution() const { return mRestitution; }
			inline float getFriction() const { return mFriction; }

			inline float getInverseMass() const { return mInverseMass; }
			inline Matrix getInverseWorldSpaceInertiaTensor() const { return mInverseInertiaTensorWorld; }
			inline Vector3 getLinearVelocity() const { return mLinearVelocity; }
			inline Vector3 getAngularVelocity() const { return mAngularVelocity; }

			inline bool active() const { return mActive; }
			inline void setActive(bool active)
			{
				mActive = active;
				if (!active)
					mMotion = 0;
			}

			inline float motion() const { return mMotion; }

			inline unsigned short islandID() const { return mIslandID; }
			inline void setIslandID(unsigned short id)
			{
				mIslandID = id;
			}
		};
	}
}

#endif
