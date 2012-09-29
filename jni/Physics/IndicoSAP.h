
#ifndef _INDICOSAP_H_
#define _INDICOSAP_H_

#include "Physics/IndicoBroadphase.h"
#include "Physics/IndicoPairCache.h"

namespace Indico
{
	namespace Physics
	{
		class SAP : public Broadphase
		{
		public:
			enum Axis
			{
				AXIS_X,
				AXIS_Y,
				AXIS_Z
			};

			class SAPObject;

			struct SAPEntry
			{
				SAPObject* Obj;
				float Value;
				bool Minima;
				uint Index;

				SAPEntry(SAPObject* obj, float value, bool minima) : Obj(obj), Value(value), Minima(minima) { }
			};

			class SAPObject
			{
			public:
				CollisionObject* mObj;
				uint mIndex;
				SAPEntry *mMinX, *mMaxX;
				SAPEntry *mMinY, *mMaxY;
				SAPEntry *mMinZ, *mMaxZ;

				SAPObject(CollisionObject* obj) : mObj(obj), mIndex(0), mMinX(0), mMaxX(0), mMinY(0), mMaxY(0), mMinZ(0), mMaxZ(0) { }
			};

		private:
			PairCache* mCache;

			SAPObject** mObjects;
			unsigned int mNumObjects;
			unsigned int mFirstFreeObjectIndex;

			unsigned int mMaxEntries;

			SAPEntry** mX;
			SAPEntry** mY;
			SAPEntry** mZ;

		public:
			SAP(unsigned int maxObjects);
			~SAP();

			int add(CollisionObject* o);
			void remove(int o);
			void update(int o);

			const BroadphasePair* getOverlappingPairs();
			unsigned int getNumOverlappingPairs();

		private:
			void insertEntries(SAPObject* obj);
			void removeEntries(SAPObject* obj);
			void updateEntries(SAPObject* obj);

			void sortUp(Axis axis, unsigned int index, bool updateOverlap);
			void sortDown(Axis axis, unsigned int index, bool updateOverlap);
		};
	}
}

#endif
