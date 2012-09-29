
#ifndef _INDICOCONTACTMANIFOLD_H_
#define _INDICOCONTACTMANIFOLD_H_

#include "Physics/IndicoCollisionObject.h"
#include "Physics/IndicoContact.h"

namespace Indico
{
	namespace Physics
	{
		class Simulator;

		class ContactManifold
		{
		private:
			Simulator* mSimulator;
			CollisionObject* mA;
			CollisionObject* mB;
			Contact** mContacts;

		public:
			ContactManifold(CollisionObject* a, CollisionObject* b);
			ContactManifold(CollisionObject* a, CollisionObject* b, Contact* c);
			~ContactManifold();

			void add(Contact* c);
			void clear();
			void update(float dt);

			inline CollisionObject* getA() { return mA; }
			inline CollisionObject* getB() { return mB; }

			inline unsigned int numContacts() { return 5; }
			inline Contact* getContact(unsigned int index) { return mContacts[index]; }
		};
	}
}

#endif
