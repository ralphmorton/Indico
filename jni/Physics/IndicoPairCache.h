
#ifndef _INDICOPAIRCACHE_H_
#define _INDICOPAIRCACHE_H_

#include "Physics/IndicoBroadphase.h"

namespace Indico
{
	namespace Physics
	{
		class PairCache
		{
		private:
			unsigned int* mHashTable;
			unsigned int* mNext;
			Broadphase::BroadphasePair* mPairs;
			unsigned int mHashSize;
			unsigned int mMask;
			unsigned int mNumPairs;

		public:
			PairCache();
			~PairCache();

			Broadphase::BroadphasePair* add(unsigned int ida, unsigned int idb);
			Broadphase::BroadphasePair* find(unsigned int ida, unsigned int idb);
			void remove(unsigned int ida, unsigned int idb);
			void clear();

			inline const Broadphase::BroadphasePair *getPairs() { return mPairs; }
			inline unsigned int getNumPairs() { return mNumPairs; }

			inline unsigned int getIndex(const Broadphase::BroadphasePair* pair)
			{
				return ((unsigned int)(((unsigned int)(pair) - (unsigned int)(mPairs))) / sizeof(Broadphase::BroadphasePair));
			}

		private:
			inline unsigned int hash(unsigned int key)
			{
				key += ~(key << 15);
				key ^=  (key >> 10);
				key +=  (key << 3);
				key ^=  (key >> 6);
				key += ~(key << 11);
				key ^=  (key >> 16);
				return key;
			}

			inline unsigned int hash(unsigned int id1, unsigned int id2)
			{
				return hash((id1&0xffff)|(id2<<16));
			}

			inline int nextPowerOfTwo(int x)
			{
				x |= (x >> 1);
				x |= (x >> 2);
				x |= (x >> 4);
				x |= (x >> 8);
				x |= (x >> 16);
				return x+1;
			}
		};
	}
}

#endif
