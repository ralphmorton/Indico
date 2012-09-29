
#ifndef _INDICOCOLLISIONOBJECT_H_
#define _INDICOCOLLISIONOBJECT_H_

#include "IndicoAABB.h"
#include "IndicoMatrix.h"
#include "IndicoTransform.h"
#include "IndicoVector3.h"
#include "Physics/IndicoCollisionShape.h"

namespace Indico
{
	namespace Physics
	{
		class RigidBody;

		class CollisionObject
		{
		private:
			friend class RigidBody;
			friend class CollisionWorld;
			friend class Simulator;

		private:
			static unsigned short mIDAlloc;

		private:
			RigidBody* mBody;
			const CollisionShape& mShape;
			Transform mTransform;
			AABB mBounds;

			int mBroadphaseHandle;
			bool mNeedsBroadphaseUpdate;

			unsigned short mID;
			CollisionObject **mContacts;
			unsigned short mNumContacts;

		public:
			CollisionObject(const Transform& trans, const CollisionShape& shape);
			~CollisionObject();

			Vector3 supportMap(const Vector3& dir);

			void update();

			inline bool isStatic() const { return !mBody; }
			inline RigidBody* getBody() { return mBody; }
			inline Transform& getTransform() { return mTransform; }
			inline const AABB& getBounds() const { return mBounds; }
		};
	}
}

#endif
