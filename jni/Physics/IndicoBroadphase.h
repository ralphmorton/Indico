
#ifndef _INDICOBROADPHASE_H_
#define _INDICOBROADPHASE_H_

namespace Indico
{
	namespace Physics
	{
		class CollisionObject;

		class Broadphase
		{
		public:
			struct BroadphasePair
			{
				unsigned int IDA, IDB;
				CollisionObject* A;
				CollisionObject* B;

				BroadphasePair() : IDA(0), IDB(0), A(0), B(0) { }
			};

		public:
			virtual int add(CollisionObject* o) = 0;
			virtual void remove(int o) = 0;
			virtual void update(int o) = 0;

			virtual const BroadphasePair* getOverlappingPairs() = 0;
			virtual unsigned int getNumOverlappingPairs() = 0;
		};
	}
}

#endif
