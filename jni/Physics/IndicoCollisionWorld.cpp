
#include "Physics/IndicoCollisionWorld.h"
#include "Physics/IndicoContactManifold.h"
#include "Physics/IndicoRigidBody.h"
#include "Physics/IndicoSimulator.h"

#include <assert.h>

namespace Indico
{
	namespace Physics
	{
		CollisionWorld::CollisionWorld(Broadphase& broadphase) : mManifoldBuckets(100), mManifoldLookup(new LinkedList<ContactManifold*>[100]), mBroadphase(broadphase), mMpr(this, 128, 1e-3f)
		{
			mSimulator = Simulator::getPtr();
		}

		CollisionWorld::~CollisionWorld()
		{
			for (int i = -1; ++i < mManifolds.size();)
				delete mManifolds[i];

			delete [] mManifoldLookup;
		}

		void CollisionWorld::addContact(CollisionObject* a, CollisionObject* b, Contact* c)
		{
			assert(a != b);

			ContactManifold* m = getManifold(a, b);

			if (m->getA() == b)
			{
				c->mNormal = -c->mNormal;
				Vector3 r1 = c->mRcp1;
				c->mRcp1 = c->mRcp2;
				c->mRcp2 = r1;
			}
			m->add(c);
		}

		void CollisionWorld::addObject(CollisionObject* obj)
		{
			int handle = mBroadphase.add(obj);
			obj->mBroadphaseHandle = handle;

			mObjects.add(obj);
		}

		void CollisionWorld::removeObject(CollisionObject* obj)
		{
			for (int i = mObjects.size(); --i >= 0;)
			{
				CollisionObject* o = mObjects[(unsigned int)i];
				if (o == obj)
				{
					mBroadphase.remove(obj->mBroadphaseHandle);
					mObjects.remove(i);
					break;
				}
			}

			for (int i = mManifolds.size(); --i >= 0;)
			{
				ContactManifold* manifold = mManifolds[(unsigned int)i];
				if (manifold->getA() == obj || manifold->getB() == obj)
				{
					deregisterContact(manifold->getA(), manifold->getB());
					removeManifoldLookup(manifold->getA(), manifold->getB());

					mManifolds.remove(i);
					delete manifold;
				}
			}
		}

		void CollisionWorld::clear()
		{
			for (int i = mObjects.size(); --i >= 0;)
				removeObject(mObjects[(unsigned int)i]);
		}

		void CollisionWorld::buildContacts(float dt)
		{
			CollisionObject *a, *b;

			for (int i = mManifolds.size(); --i >= 0;)
			{
				ContactManifold* manifold = mManifolds[(unsigned int)i];
				a = manifold->getA();
				b = manifold->getB();

				if (((!a->mBody || !a->mBody->active()) && (!b->mBody || !b->mBody->active())) || !a->getBounds().intersects(b->getBounds()))
				{
					deregisterContact(a, b);
					removeManifoldLookup(a, b);

					mManifolds.remove(i);
					delete manifold;
					continue;
				}

				manifold->update(dt);
			}

			float threshold = mSimulator->getRestingVelocityLimit();

			unsigned short islandIDGen = 0;
			for (int i = -1; ++i < mObjects.size();)
			{
				a = mObjects[i];

				if (!a->mBody || !a->mBody->active())
					continue;

				if (a->mBody && !a->mBody->islandID())
				{
					islandIDGen++;
					if (!islandActive(a, threshold, islandIDGen))
						deactivateIsland(a, islandIDGen);
				}

				if (a->mNeedsBroadphaseUpdate)
				{
					mBroadphase.update(a->mBroadphaseHandle);
					a->mNeedsBroadphaseUpdate = false;
				}
			}

			const Broadphase::BroadphasePair* pairs = mBroadphase.getOverlappingPairs();
			uint numpairs = mBroadphase.getNumOverlappingPairs();

			for (int i = -1; ++i < numpairs;)
			{
				a = pairs[i].A;
				b = pairs[i].B;

				if ((!a->mBody || !a->getBody()->active()) && (!b->mBody || !b->getBody()->active()))
					continue;

				mMpr.collide(pairs[i].A, pairs[i].B);
			}
		}

		const Vector<ContactManifold*>& CollisionWorld::getManifolds() const
		{
			return mManifolds;
		}

		void CollisionWorld::clearManifolds()
		{
			for (int i = -1; ++i < mManifolds.size();)
				delete mManifolds[i];
			mManifolds.clear();
		}

		ContactManifold* CollisionWorld::findManifold(CollisionObject* a, CollisionObject* b)
		{
			int ia = a->mID;
			int ib = b->mID;

			if (ib<ia)
			{
				int iat = ia;
				ia = ib;
				ib = iat;
			}

			int hval = hash(ia, ib);

			if (!mManifoldLookup[hval%mManifoldBuckets].first())
				return 0;

			LinkedListNode<ContactManifold*>* node = mManifoldLookup[hval%mManifoldBuckets].first();
			ContactManifold* m = 0;
			while (node)
			{
				m = node->item();
				if ((m->getA() == a && m->getB() == b) || (m->getA() == b && m->getB() == a))
					return m;

				node = node->next();
			}

			return 0;
		}

		ContactManifold* CollisionWorld::getManifold(CollisionObject* a, CollisionObject* b)
		{
			ContactManifold* m = findManifold(a, b);
			if (m) return m;

			int ia = a->mID;
			int ib = b->mID;

			if (ib<ia)
			{
				int iat = ia;
				ia = ib;
				ib = iat;
			}

			int hval = hash(ia, ib);

			m = new ContactManifold(a, b);

			registerContact(a, b);
			mManifolds.add(m);
			mManifoldLookup[hval%mManifoldBuckets].insertFirst(m);

			return m;
		}

		void CollisionWorld::removeManifoldLookup(CollisionObject* a, CollisionObject* b)
		{
			int ia = a->mID;
			int ib = b->mID;

			if (ib<ia)
			{
				int iat = ia;
				ia = ib;
				ib = iat;
			}

			int hval = hash(ia, ib);

			if (!mManifoldLookup[hval%mManifoldBuckets].first())
				return;

			LinkedListNode<ContactManifold*>* node = mManifoldLookup[hval%mManifoldBuckets].first();
			ContactManifold* m = 0;
			while (node)
			{
				m = node->item();
				if ((m->getA() == a && m->getB() == b) || (m->getA() == b && m->getB() == a))
				{
					mManifoldLookup[hval%mManifoldBuckets].remove(node);
					return;
				}

				node = node->next();
			}
		}

		void CollisionWorld::registerContact(CollisionObject* a, CollisionObject* b)
		{
			bool upd = true;
			bool done = false;

			for (int i = -1; ++i < a->mNumContacts;)
				if (a->mContacts[i] == b) { upd = false; break; }

			if (upd)
			{
				for (int i = -1; ++i < a->mNumContacts;)
				{
					if (!a->mContacts[i]) { a->mContacts[i] = b; done = true; break; }
				}

				if (!done)
				{
					CollisionObject** contacts = new CollisionObject*[++a->mNumContacts];
					contacts[a->mNumContacts-1] = b;

					for (int i = -1; ++i < a->mNumContacts-1;)
						contacts[i] = a->mContacts[i];

					delete [] a->mContacts;
					a->mContacts = contacts;
				}
			}

			upd = true;
			done = false;

			for (int i = -1; ++i < b->mNumContacts;)
				if (b->mContacts[i] == a) { upd = false; break; }

			if (upd)
			{
				for (int i = -1; ++i < b->mNumContacts;)
				{
					if (!b->mContacts[i]) { b->mContacts[i] = a; done = true; break; }
				}
				if (!done)
				{
					CollisionObject** contacts = new CollisionObject*[++b->mNumContacts];
					contacts[b->mNumContacts-1] = a;

					for (int i = -1; ++i < b->mNumContacts-1;)
						contacts[i] = b->mContacts[i];

					delete [] b->mContacts;
					b->mContacts = contacts;
				}
			}
		}

		void CollisionWorld::deregisterContact(CollisionObject* a, CollisionObject* b)
		{
			for (int i = -1; ++i < a->mNumContacts;)
				if (a->mContacts[i] == b) { a->mContacts[i] = 0; break; }

			for (int i = -1; ++i < b->mNumContacts;)
				if (b->mContacts[i] == a) { b->mContacts[i] = 0; break; }
		}

		bool CollisionWorld::islandActive(CollisionObject* obj, float threshold, unsigned short islandID)
		{
			if (obj->mBody->motion() > threshold)
				return true;

			obj->mBody->setIslandID(islandID);

			for (int i = -1; ++i < obj->mNumContacts;)
			{
				CollisionObject *obj2 = obj->mContacts[i];
				if (obj2 && obj2->mBody && !obj2->mBody->islandID())
				{
					if (obj2->mBody->motion() > threshold)
						return true;

					if (islandActive(obj2, threshold, islandID))
						return true;
				}
			}

			return false;
		}

		void CollisionWorld::deactivateIsland(CollisionObject *obj, unsigned short islandID)
		{
			obj->mBody->setActive(false);

			for (int i = -1; ++i < obj->mNumContacts;)
			{
				CollisionObject *obj2 = obj->mContacts[i];
				if (obj2 && obj2->mBody && obj2->mBody->islandID() == islandID && obj2->mBody->active())
					deactivateIsland(obj2, islandID);
			}
		}
	}
}
