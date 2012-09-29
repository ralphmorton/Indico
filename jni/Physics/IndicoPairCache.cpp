
#include "Physics/IndicoPairCache.h"

#include <assert.h>
#include <string.h>

namespace Indico
{
	namespace Physics
	{
		PairCache::PairCache() : mHashSize(0), mMask(0), mHashTable(0), mNext(0), mNumPairs(0), mPairs(0)
		{

		}

		PairCache::~PairCache()
		{
			clear();
		}

		Broadphase::BroadphasePair* PairCache::find(unsigned int ida, unsigned int idb)
		{
			if (!mHashTable) return 0;

			if (ida>idb)
			{
				int idt = ida;
				ida = idb;
				idb = idt;
			}

			const unsigned int hvalue = hash(ida, idb) & mMask;

			unsigned int offset = mHashTable[hvalue];
			while(offset != 0xffffffff && (mPairs[offset].IDA != ida || mPairs[offset].IDB != idb))
			{
				assert(mPairs[offset].IDA != 0xffffffff);
				offset = mNext[offset];
			}

			if(offset == 0xffffffff) return 0;

			assert(offset<mNumPairs);

			return &mPairs[offset];
		}

		Broadphase::BroadphasePair *PairCache::add(unsigned int ida, unsigned int idb)
		{
			if (ida>idb)
			{
				int idt = ida;
				ida = idb;
				idb = idt;
			}

			unsigned int hvalue = hash(ida, idb) & mMask;

			Broadphase::BroadphasePair *pair = find(ida, idb);
			if (pair) return pair;

			if (mNumPairs >= mHashSize)
			{
				mHashSize = nextPowerOfTwo(mNumPairs+1);
				mMask = mHashSize - 1;

				delete [] mHashTable;
				mHashTable = new unsigned int[mHashSize];

				for(unsigned int i = 0; i < mHashSize; i++) mHashTable[i] = 0xffffffff;

				Broadphase::BroadphasePair* newpairs = new Broadphase::BroadphasePair[mHashSize];
				unsigned int* newnext = new unsigned int[mHashSize];

				if (mNumPairs) memcpy(newpairs, mPairs, mNumPairs * sizeof(Broadphase::BroadphasePair));

				for (unsigned int i = 0; i < mNumPairs; i++)
				{
					unsigned int hval = hash(mPairs[i].IDA, mPairs[i].IDB) & mMask;
					newnext[i] = mHashTable[hval];
					mHashTable[hval] = i;
				}

				delete [] mNext;
				delete [] mPairs;

				mPairs = newpairs;
				mNext = newnext;

				hvalue = hash(ida, idb) & mMask;
			}

			Broadphase::BroadphasePair* p = &mPairs[mNumPairs];
			p->IDA = ida;
			p->IDB = idb;

			mNext[mNumPairs] = mHashTable[hvalue];
			mHashTable[hvalue] = mNumPairs++;

			return p;
		}

		void PairCache::remove(unsigned int ida, unsigned int idb)
		{
			if (ida>idb)
			{
				int idt = ida;
				ida = idb;
				idb = idt;
			}

			const unsigned int hvalue = hash(ida, idb) & mMask;

			const Broadphase::BroadphasePair *pair = find(ida, idb);
			if (!pair) return;

			const unsigned int index = getIndex(pair);
			unsigned int offset = mHashTable[hvalue];
			unsigned int previous = 0xffffffff;

			while (offset != index)
			{
				previous = offset;
				offset = mNext[offset];
			}

			if (previous != 0xffffffff)
				mNext[previous] = mNext[index];
			else
				mHashTable[hvalue] = mNext[index];

			if (1)
			{
				const unsigned int lastindex = mNumPairs - 1;

				if (lastindex == index)
				{
					mNumPairs--;
					return;
				}

				const Broadphase::BroadphasePair* last = &mPairs[lastindex];
				const unsigned int lasthvalue = hash(last->IDA, last->IDB) & mMask;

				unsigned int offset = mHashTable[lasthvalue];

				unsigned int previous = 0xffffffff;
				while(offset != lastindex)
				{
					previous = offset;
					offset = mNext[offset];
				}

				if(previous != 0xffffffff)
					mNext[previous] = mNext[lastindex];
				else
					mHashTable[lasthvalue] = mNext[lastindex];

				mPairs[index] = mPairs[lastindex];
				mNext[index] = mHashTable[lasthvalue];
				mHashTable[lasthvalue] = index;

				mNumPairs--;
			}
		}

		void PairCache::clear()
		{
			delete [] mHashTable;
			delete [] mNext;
			delete [] mPairs;

			mHashSize = 0;
			mMask = 0;
			mNumPairs = 0;
		}
	}
}
