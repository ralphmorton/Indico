
#include "IndicoVector.h"
#include "IndicoVector3.h"
#include "Physics/IndicoSAP.h"
#include "Physics/IndicoCollisionObject.h"
#include "Physics/IndicoPairCache.h"

namespace Indico
{
	namespace Physics
	{
		SAP::SAP(unsigned int maxObjects) : mFirstFreeObjectIndex(0), mNumObjects(0)
		{
			mCache = new PairCache();

			mMaxEntries = maxObjects * 2;

			mX = new SAPEntry*[mMaxEntries];
			mY = new SAPEntry*[mMaxEntries];
			mZ = new SAPEntry*[mMaxEntries];

			for (int i = -1; ++i < mMaxEntries; mX[i] = mY[i] = mZ[i] = 0);;

			mObjects = new SAPObject*[maxObjects];

			for (int i = -1; ++i < maxObjects; mObjects[i] = 0);;
		}

		SAP::~SAP()
		{
			for (int i = -1; ++i <  mNumObjects * 2;)
			{
				delete mX[i];
				delete mY[i];
				delete mZ[i];
			}

			delete [] mX;
			delete [] mY;
			delete [] mZ;

			delete [] mObjects;

			delete mCache;
		}

		int SAP::add(CollisionObject *o)
		{
			SAPObject *obj = new SAPObject(o);

			int handle = mFirstFreeObjectIndex;
			mObjects[handle] = obj;
			obj->mIndex = handle;

			while (mObjects[mFirstFreeObjectIndex] && mFirstFreeObjectIndex < mMaxEntries/2) mFirstFreeObjectIndex++;

			mNumObjects++;
			insertEntries(obj);

			return handle;
		}

		void SAP::remove(int o)
		{
			SAPObject* obj = mObjects[o];

			mObjects[o] = 0;
			if (o < mFirstFreeObjectIndex) mFirstFreeObjectIndex = o;

			removeEntries(obj);
			mNumObjects--;
			delete obj;
		}

		void SAP::update(int o)
		{
			SAPObject* obj = mObjects[o];
			updateEntries(obj);
		}

		const Broadphase::BroadphasePair* SAP::getOverlappingPairs()
		{
			return mCache->getPairs();
		}

		uint SAP::getNumOverlappingPairs()
		{
			return mCache->getNumPairs();
		}

		void SAP::insertEntries(SAPObject* obj)
		{
			AABB bounds = obj->mObj->getBounds();
			Vector3 min = bounds.mMin;
			Vector3 max = bounds.mMax;

			obj->mMinX = new SAPEntry(obj, min.x, true);
			obj->mMaxX = new SAPEntry(obj, max.x, false);
			obj->mMinY = new SAPEntry(obj, min.y, true);
			obj->mMaxY = new SAPEntry(obj, max.y, false);
			obj->mMinZ = new SAPEntry(obj, min.z, true);
			obj->mMaxZ = new SAPEntry(obj, max.z, false);

			uint insertindex = (mNumObjects * 2) - 1;

			mX[insertindex-1] = obj->mMinX;
			mX[insertindex] = obj->mMaxX;
			mY[insertindex-1] = obj->mMinY;
			mY[insertindex] = obj->mMaxY;
			mZ[insertindex-1] = obj->mMinZ;
			mZ[insertindex] = obj->mMaxZ;

			obj->mMinX->Index = insertindex-1;
			obj->mMaxX->Index = insertindex;
			obj->mMinY->Index = insertindex-1;
			obj->mMaxY->Index = insertindex;
			obj->mMinZ->Index = insertindex-1;
			obj->mMaxZ->Index = insertindex;

			sortDown(AXIS_X, insertindex-1, false);
			sortDown(AXIS_X, insertindex, false);
			sortDown(AXIS_Y, insertindex-1, false);
			sortDown(AXIS_Y, insertindex, false);
			sortDown(AXIS_Z, insertindex-1, true);
			sortDown(AXIS_Z, insertindex, true);
		}

		void SAP::removeEntries(SAPObject* obj)
		{
			uint numEntries = mNumObjects * 2;

			//REMOVE OVERLAPS

			Vector<unsigned int> collisionsZ;
			for (unsigned int i = obj->mMinZ->Index; ++i < obj->mMaxZ->Index;)
				collisionsZ.add(mZ[i]->Obj->mIndex);

			for (unsigned int i = 0; +i < collisionsZ.size(); i++)
				mCache->remove(obj->mIndex, collisionsZ[i]);

			//REMOVE ENDPOINTS

			unsigned int iminx = obj->mMinX->Index;
			for (int i = iminx-1; ++i < numEntries-1;)
			{
				mX[i] = mX[i+1];
				mX[i]->Index = i;
			}

			unsigned int imaxx = obj->mMaxX->Index;
			for (int i = imaxx-1; ++i < numEntries-1;)
			{
				mX[i] = mX[i+1];
				mX[i]->Index = i;
			}

			unsigned int iminy = obj->mMinY->Index;
			for (int i = iminy-1; ++i < numEntries-1;)
			{
				mY[i] = mY[i+1];
				mY[i]->Index = i;
			}

			unsigned int imaxy = obj->mMaxY->Index;
			for (int i = imaxy-1; ++i < numEntries-1;)
			{
				mY[i] = mY[i+1];
				mY[i]->Index = i;
			}

			unsigned int iminz = obj->mMinZ->Index;
			for (int i = iminz-1; ++i < numEntries-1;)
			{
				mZ[i] = mZ[i+1];
				mZ[i]->Index = i;
			}

			unsigned int imaxz = obj->mMaxZ->Index;
			for (int i = imaxz-1; ++i < numEntries-1;)
			{
				mZ[i] = mZ[i+1];
				mZ[i]->Index = i;
			}

			mX[numEntries-1] = 0;
			mX[numEntries-2] = 0;
			mY[numEntries-1] = 0;
			mY[numEntries-2] = 0;
			mZ[numEntries-1] = 0;
			mZ[numEntries-2] = 0;

			delete obj->mMinX;
			delete obj->mMaxX;
			delete obj->mMinY;
			delete obj->mMaxY;
			delete obj->mMinZ;
			delete obj->mMaxZ;

		}

		void SAP::updateEntries(SAPObject* obj)
		{
			unsigned int numEntries = mNumObjects * 2;

			AABB bounds = obj->mObj->getBounds();
			Vector3 min = bounds.mMin;
			Vector3 max = bounds.mMax;

			float ominx = obj->mMinX->Value;
			float omaxx = obj->mMaxX->Value;
			float ominy = obj->mMinY->Value;
			float omaxy = obj->mMaxY->Value;
			float ominz = obj->mMinZ->Value;
			float omaxz = obj->mMaxZ->Value;

			obj->mMinX->Value = min.x;
			if (min.x < ominx) sortDown(AXIS_X, obj->mMinX->Index, true); else sortUp(AXIS_X, obj->mMinX->Index, true);
			obj->mMaxX->Value = max.x;
			if (max.x < omaxx) sortDown(AXIS_X, obj->mMaxX->Index, true); else sortUp(AXIS_X, obj->mMaxX->Index, true);
			obj->mMinY->Value = min.y;
			if (min.y < ominy) sortDown(AXIS_Y, obj->mMinY->Index, true); else sortUp(AXIS_Y, obj->mMinY->Index, true);
			obj->mMaxY->Value = max.y;
			if (max.y < omaxy) sortDown(AXIS_Y, obj->mMaxY->Index, true); else sortUp(AXIS_Y, obj->mMaxY->Index, true);
			obj->mMinZ->Value = min.z;
			if (min.z < ominz) sortDown(AXIS_Z, obj->mMinZ->Index, true); else sortUp(AXIS_Z, obj->mMinZ->Index, true);
			obj->mMaxZ->Value = max.z;
			if (max.z < omaxz) sortDown(AXIS_Z, obj->mMaxZ->Index, true); else sortUp(AXIS_Z, obj->mMaxZ->Index, true);
		}

		void SAP::sortUp(Axis axis, unsigned int index, bool updateOverlap)
		{
			SAPEntry** entries = (axis==AXIS_X) ? mX : (axis==AXIS_Y) ? mY : mZ;

			unsigned int total = mNumObjects * 2;

			SAPEntry *entry = entries[index];
			bool minima = entry->Minima;

			if (index == total - 1) return;
			if (entries[index+1]->Value > entry->Value) return; //Already sorted

			for (uint i = index; ++i < total;)
			{
				if (entries[i]->Value > entry->Value)
				{
					unsigned int newindex = i - 1;
					unsigned int sortindex = index;

					while (sortindex < newindex)
					{
						int dest = sortindex;
						int src = ++sortindex;
						entries[dest] = entries[src];
						entries[dest]->Index = dest;

						if (updateOverlap && !minima && entries[dest]->Minima && entry->Obj != entries[dest]->Obj && entry->Obj->mObj->getBounds().intersects(entries[dest]->Obj->mObj->getBounds()))
						{
							Broadphase::BroadphasePair* pair = mCache->add(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
							pair->A = entry->Obj->mObj;
							pair->B = entries[dest]->Obj->mObj;
						}
						else if (updateOverlap && minima && !entries[dest]->Minima && entry->Obj != entries[dest]->Obj)
							mCache->remove(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
					}

					entries[newindex] = entry;
					entry->Index = newindex;
					return;
				}
			}

			unsigned int newindex = total - 1;
			unsigned int sortindex = index;

			while (sortindex < newindex)
			{
				int dest = sortindex;
				int src = ++sortindex;
				entries[dest] = entries[src];
				entries[dest]->Index = dest;

				if (updateOverlap && !minima && entries[dest]->Minima && entry->Obj != entries[dest]->Obj && entry->Obj->mObj->getBounds().intersects(entries[dest]->Obj->mObj->getBounds()))
				{
					Broadphase::BroadphasePair *pair = mCache->add(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
					pair->A = entry->Obj->mObj;
					pair->B = entries[dest]->Obj->mObj;
				}
				else if (updateOverlap && minima && !entries[dest]->Minima && entry->Obj != entries[dest]->Obj)
					mCache->remove(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
			}

			entries[newindex] = entry;
			entry->Index = newindex;
			return;
		}

		void SAP::sortDown(Axis axis, unsigned int index, bool updateOverlap)
		{
			SAPEntry** entries = (axis==AXIS_X) ? mX : (axis==AXIS_Y) ? mY : mZ;

			unsigned int total = mNumObjects * 2;

			SAPEntry *entry = entries[index];
			bool minima = entry->Minima;

			float eval = entry->Value;
			if (index == 0) return;
			if (entries[index-1]->Value < entry->Value) return; //Already sorted

			for (int i = index; --i >= 0;)
			{
				if (entries[i]->Value < entry->Value)
				{
					unsigned int newindex = i + 1;
					unsigned int sortindex = index;

					while (sortindex > newindex)
					{
						int dest = sortindex;
						int src = --sortindex;
						entries[dest] = entries[src];
						entries[dest]->Index = dest;

						if (updateOverlap && minima && !entries[dest]->Minima && entry->Obj != entries[dest]->Obj && entry->Obj->mObj->getBounds().intersects(entries[dest]->Obj->mObj->getBounds()))
						{
							Broadphase::BroadphasePair *pair = mCache->add(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
							pair->A = entry->Obj->mObj;
							pair->B = entries[dest]->Obj->mObj;
						}
						else if (updateOverlap && !minima && entries[dest]->Minima && entry->Obj != entries[dest]->Obj)
							mCache->remove(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
					}

					entries[newindex] = entry;
					entry->Index = newindex;
					return;
				}
			}

			unsigned int newindex = 0;
			unsigned int sortindex = index;

			while (sortindex > newindex)
			{
				int dest = sortindex;
				int src = --sortindex;
				entries[dest] = entries[src];
				entries[dest]->Index = dest;

				if (updateOverlap && minima && !entries[dest]->Minima && entry->Obj != entries[dest]->Obj && entry->Obj->mObj->getBounds().intersects(entries[dest]->Obj->mObj->getBounds()))
				{
					Broadphase::BroadphasePair *pair = mCache->add(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
					pair->A = entry->Obj->mObj;
					pair->B = entries[dest]->Obj->mObj;
				}
				else if (updateOverlap && !minima && entries[dest]->Minima && entry->Obj != entries[dest]->Obj)
					mCache->remove(entry->Obj->mIndex, entries[dest]->Obj->mIndex);
			}

			entries[newindex] = entry;
			entry->Index = newindex;
			return;
		}
	}
}
