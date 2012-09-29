
#ifndef _INDICOCOLLISIONWORLD_H_
#define _INDICOCOLLISIONWORLD_H_

#include "IndicoLinkedList.h"
#include "IndicoVector.h"
#include "Physics/IndicoBroadphase.h"
#include "Physics/IndicoCollisionObject.h"
#include "Physics/IndicoMPR.h"

namespace Indico
{
	namespace Physics
	{
		class Contact;
		class ContactManifold;
		class Simulator;

		class CollisionWorld
		{
		private:
			friend class CollisionObject;

		private:
			Vector<CollisionObject*> mObjects;
			Vector<ContactManifold*> mManifolds;
			LinkedList<ContactManifold*>* mManifoldLookup;
			unsigned int mManifoldBuckets;

			Simulator* mSimulator;
			Broadphase& mBroadphase;
			MPR mMpr;

		public:
			CollisionWorld(Broadphase& broadphase);
			~CollisionWorld();

			void addObject(CollisionObject* obj);
			void removeObject(CollisionObject* obj);

			void clear();

			void addContact(CollisionObject* a, CollisionObject* b, Contact* c);

			void buildContacts(float dt);

			const Vector<ContactManifold*>& getManifolds() const;
			void clearManifolds();

		private:
			ContactManifold* findManifold(CollisionObject* a, CollisionObject* b);
			ContactManifold* getManifold(CollisionObject* a, CollisionObject* b);
			void removeManifoldLookup(CollisionObject* a, CollisionObject* b);
			void registerContact(CollisionObject* a, CollisionObject* b);
			void deregisterContact(CollisionObject* a, CollisionObject* b);
			bool islandActive(CollisionObject* obj, float threshold, unsigned short islandID);
			void deactivateIsland(CollisionObject* obj, unsigned short islandID);

			inline int hash(int key)
			{
				key += ~(key << 15);
				key ^=  (key >> 10);
				key +=  (key << 3);
				key ^=  (key >> 6);
				key += ~(key << 11);
				key ^=  (key >> 16);
				return key;
			}

			inline int hash(int id1, int id2)
			{
				return hash((id1&0xffff)|(id2<<16));
			}
		};
	}
}

#endif
