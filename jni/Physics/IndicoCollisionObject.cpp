
#include "Physics/IndicoCollisionObject.h"
#include "Physics/IndicoRigidBody.h"

namespace Indico
{
	namespace Physics
	{
		unsigned short CollisionObject::mIDAlloc = 0;

		CollisionObject::CollisionObject(const Transform& trans, const CollisionShape& shape) : mBody(0), mTransform(trans), mShape(shape), mBroadphaseHandle(0) , mNeedsBroadphaseUpdate(false), mID(mIDAlloc++), mContacts(new CollisionObject*[3]), mNumContacts(3)
		{
			mContacts[0] = 0;
			mContacts[1] = 0;
			mContacts[2] = 0;

			update();
		}

		CollisionObject::~CollisionObject()
		{
			delete [] mContacts;
		}

		Vector3 CollisionObject::supportMap(const Vector3& direction)
		{
			Matrix imat = mTransform.getInverseMat();
			imat.m[3][0] = 0;
			imat.m[3][1] = 0;
			imat.m[3][2] = 0;

			Vector3 dir = direction * imat;

			Vector3 p = mShape.supportMap(dir);

			return p * mTransform.getMat();
		}

		void CollisionObject::update()
		{
			mBounds.mMin = Vector3(
				supportMap(-Vector3::UNIT_X).x * 1.05f,
				supportMap(-Vector3::UNIT_Y).y * 1.05f,
				supportMap(-Vector3::UNIT_Z).z * 1.05f
			);
			mBounds.mMax = Vector3(
				supportMap(Vector3::UNIT_X).x * 1.05f,
				supportMap(Vector3::UNIT_Y).y * 1.05f,
				supportMap(Vector3::UNIT_Z).z * 1.05f
			);
		}
	}
}
