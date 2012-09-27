
#ifndef _INDICOHASHMAP_H_
#define _INDICOHASHMAP_H_

#include "IndicoException.h"

#include <new>
#include <string.h>

namespace Indico
{
	template <class K, class V> class HashMap
	{
	private:
		template <class KN, class VN> class Node
		{
		public:
			KN mK;
			VN mV;
			Node<KN, VN>* mNext;

			Node(const KN& k, const VN& v) : mK(k), mV(v), mNext(0) { }
		};

	private:
		unsigned int mSeed;
		unsigned int mNumBuckets;
		Node<K, V>** mBuckets;

	private:
		HashMap(const HashMap& other);
		HashMap& operator = (const HashMap& other);

	public:
		HashMap(unsigned int seed, unsigned int buckets) : mSeed(seed), mNumBuckets(buckets)
		{
			if (mNumBuckets == 0)
				throw Exception("Minimum of one bucket");

			mBuckets = (Node<K, V>**)::operator new (sizeof(Node<K, V>*) * mNumBuckets);
			memset(mBuckets, 0, sizeof(Node<K, V>*) * mNumBuckets);
		}

		~HashMap()
		{
			clear();

			::operator delete (mBuckets);
		}

		V& operator [] (const K& k)
		{
			unsigned int ix = k.hash(mSeed) % mNumBuckets;

			Node<K, V>* n = mBuckets[ix];

			while (n)
			{
				if (n->mK == k) return n->mV;
				n = n->mNext;
			}

			if (!mBuckets[ix])
			{
				mBuckets[ix] = new Node<K, V>(k, V());
				return (mBuckets[ix])->mV;
			}

			n = mBuckets[ix];
			while (n->mNext) n = n->mNext;
			n->mNext = new Node<K, V>(k, V());

			return n->mNext->mV;
		}

		const V& operator [] (const K& k) const
		{
			unsigned int ix = k.hash(mSeed) % mNumBuckets;

			Node<K, V>* n = mBuckets[ix];

			while (n)
			{
				if (n->mK == k) return n->mV;
				n = n->mNext;
			}

			throw Exception("No such item exists");
		}

		bool has(const K& k) const
		{
			unsigned int ix = k.hash(mSeed) % mNumBuckets;

			Node<K, V>* n = mBuckets[ix];

			while (n)
			{
				if (n->mK == k) return true;
				n = n->mNext;
			}

			return false;
		}

		bool remove(const K& k)
		{
			unsigned int ix = k.hash(mSeed) % mNumBuckets;

			Node<K, V>* ln = 0;
			Node<K, V>* n = mBuckets[ix];

			while (n)
			{
				if (n->mK == k)
				{
					if (ln)
						ln->mNext = n->mNext;
					else
						mBuckets[ix] = n->mNext;

					delete n;
					return true;
				}

				ln = n;
				n = n->mNext;
			}

			return false;
		}

		void clear()
		{
			for (unsigned int i = 0; i < mNumBuckets; i++)
			{
				Node<K, V>* n = mBuckets[i];
				while (n)
				{
					Node<K, V>* nd = n;
					n = n->mNext;
					delete nd;
				}
			}
		}
	};
}

#endif
