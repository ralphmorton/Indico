
#include "Physics/IndicoContactManifold.h"
#include "Physics/IndicoRigidBody.h"
#include "Physics/IndicoSimulator.h"

namespace Indico
{
	namespace Physics
	{
		ContactManifold::ContactManifold(CollisionObject* a, CollisionObject* b) : mA(a), mB(b)
		{
			mSimulator = Simulator::getPtr();

			mContacts = new Contact*[5];
			mContacts[0] = 0;
			mContacts[1] = 0;
			mContacts[2] = 0;
			mContacts[3] = 0;
			mContacts[4] = 0;
		}

		ContactManifold::ContactManifold(CollisionObject* a, CollisionObject* b, Contact* c) : mA(a), mB(b)
		{
			mSimulator = Simulator::getPtr();

			mContacts = new Contact*[5];
			mContacts[0] = c;
			mContacts[1] = 0;
			mContacts[2] = 0;
			mContacts[3] = 0;
			mContacts[4] = 0;
		}

		ContactManifold::~ContactManifold()
		{
			clear();
			delete [] mContacts;
		}

		void ContactManifold::clear()
		{
			for (int i = -1; ++i < 5;)
				if (mContacts[i]) delete mContacts[i];

			mContacts[0] = 0;
			mContacts[1] = 0;
			mContacts[2] = 0;
			mContacts[3] = 0;
			mContacts[4] = 0;
		}

		void ContactManifold::add(Contact* c)
		{
			float minPen = 9999999999.f;
			int minIndex = 0;

			for (int i = -1; ++i < 5;)
			{
				if (mContacts[i])
				{
					Contact* existing = mContacts[i];

					float r1 = (existing->mRcp1 - c->mRcp1).lengthSquared();
					float r2 = (existing->mRcp2 - c->mRcp2).lengthSquared();

					if (r1 < 0.5e-1f && r2 < 0.5e-1f)
					{
						c->mPN = existing->mPN;
						c->mPNB = existing->mPNB;
						c->mPT = existing->mPT;

						delete mContacts[i];
						mContacts[i] = c;

						return;
					}

					if (existing->mPenetration <= minPen)
					{
						minPen = existing->mPenetration;
						minIndex = i;
					}
				}
			}

			for (int i = -1; ++i < 5;)
			{
				if (!mContacts[i])
				{
					mContacts[i] = c;
					return;
				}
			}

			if (c->mPenetration < minPen)
			{
				delete c;
				return;
			}

			delete mContacts[minIndex];
			mContacts[minIndex] = c;
		}

		void ContactManifold::update(float dt)
		{
			float maxsep = mSimulator->getMaxContactSeparation();

			bool update = false;

			RigidBody *ra = mA->getBody();
			RigidBody *rb = mB->getBody();

			if ((ra && ra->getAngularVelocity().length()) || (rb && rb->getAngularVelocity().length()))
				update = true;

			if (update)
			{
				float minp = 99999999.f;
				int mini = 0;
				int cntc = 0;

				for (int i = -1; ++i < 5;)
				{
					Contact *c = mContacts[i];
					if (!c) continue;

					Vector3 ba = (c->mRcp1 + mA->getTransform().getTranslation()) - (c->mRcp2 + mB->getTransform().getTranslation());
					c->mPenetration = ba.dot(c->mNormal);

					if (c->mPenetration < -maxsep || ((c->mNormal * c->mPenetration) - ba).length() > maxsep)
					{
						delete c;
						mContacts[i] = 0;
						continue;
					}

					cntc++;
					if (c->mPenetration < minp)
					{
						minp = c->mPenetration;
						mini = i;
					}
				}

				if (cntc > 4)
				{
					delete mContacts[mini];
					mContacts[mini] = 0;
				}
			}
		}
	}
}
