
#include "Physics/IndicoSimulator.h"
#include "Physics/IndicoRigidBody.h"

namespace Indico
{
	namespace Physics
	{
		template<> Simulator* Singleton<Simulator>::mSingleton = 0;

		Simulator::Simulator(Broadphase& broadphase) : mCollisionWorld(broadphase), mGravity(Vector3::UNIT_Y*-10), mMaxContactSeparation(0.005f), mAllowedPenetration(0.005f), mCollisionBiasFactor(0.2f), mRestingVelocityLimit(0.08f), mIterations(10)
		{

		}

		Simulator::~Simulator()
		{

		}

		void Simulator::addBody(RigidBody* body)
		{
			mBodies.add(body);
			mCollisionWorld.addObject(body->mObject);
		}

		void Simulator::addCollisionObject(CollisionObject* obj)
		{
			mCollisionWorld.addObject(obj);
		}

		void Simulator::removeBody(RigidBody* body)
		{
			mCollisionWorld.removeObject(body->mObject);

			for (int i = mBodies.size(); --i >= 0;)
			{
				RigidBody* b = mBodies[(unsigned int)i];
				if (b == body)
				{
					mBodies.remove(i);
					return;
				}
			}
		}

		void Simulator::removeCollisionObject(CollisionObject *obj)
		{
			mCollisionWorld.removeObject(obj);
		}

		void Simulator::clear()
		{
			for (int i =  mBodies.size(); --i >= 0;)
				removeBody(mBodies[i]);

			mCollisionWorld.clear();
		}

		void Simulator::step(float dt)
		{
			for (int i = -1; ++i < mBodies.size();)
			{
				RigidBody* b = mBodies[(unsigned int)i];
				b->setIslandID(0);

				if (!b->active()) continue;

				Vector3 linVel = b->getInverseMass() * b->mForce;
				linVel += mGravity;

				b->mLinearVelocity += dt * linVel;
				b->mAngularVelocity += (dt * b->mTorque) * b->getInverseWorldSpaceInertiaTensor();

				b->mLinearVelocity *= Math::clamp(1.0f - dt * b->mLinearDamping, 0.0f, 1.0f);
				b->mAngularVelocity *= Math::clamp(1.0f - dt * b->mAngularDamping, 0.0f, 1.0f);

				b->mBiasAngularVelocity = Vector3::ZERO;
				b->mBiasLinearVelocity = Vector3::ZERO;
			}

			mCollisionWorld.buildContacts(dt);

			const Vector<ContactManifold*>& manifolds = mCollisionWorld.getManifolds();

			mCollisionSolver.prep(dt, manifolds);

			for (int i = -1; ++i < mIterations;)
				mCollisionSolver.step(manifolds);

			float mthreshold = 8.0f * mRestingVelocityLimit;

			for (int i = -1; ++i < mBodies.size();)
			{
				RigidBody* b = mBodies[i];
				Transform& t = b->mObject->getTransform();

				float svel = b->mLinearVelocity.lengthSquared() + b->mAngularVelocity.lengthSquared();

				if (svel > mRestingVelocityLimit)
				{
					b->setActive(true);
					b->mMotion = mthreshold;
				}
				else if (!b->active()) continue;

				Vector3 linveld = (b->mLinearVelocity + b->mBiasLinearVelocity) * dt;
				t.setTranslation(t.getTranslation() + linveld);

				Quaternion orientation = t.getOrientation();
				Vector3 vp = (b->mAngularVelocity + b->mBiasAngularVelocity) * dt;
				Quaternion qn;
				qn.x = vp.x;
				qn.y = vp.y;
				qn.z = vp.z;
				qn.w = 0;
				qn = qn * orientation;
				orientation.x += qn.x * 0.5f;
				orientation.y += qn.y * 0.5f;
				orientation.z += qn.z * 0.5f;
				orientation.w += qn.w * 0.5f;
				orientation.normalise();

				t.setOrientation(orientation);

				b->mForce = Vector3::ZERO;
				b->mTorque = Vector3::ZERO;

				t.update();
				b->update();

				if (linveld.lengthSquared() > 0.0f || vp.lengthSquared() > 0.0f)
					b->mObject->mNeedsBroadphaseUpdate = true;

				float bias = Math::pow(0.5f, dt);
				b->mMotion = Math::max(0.0f, bias * b->mMotion + (1.0f - bias) * svel);
				b->mMotion = Math::min(mthreshold, b->mMotion);
			}
		}
	}
}
