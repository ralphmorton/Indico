
#ifndef _INDICOVECTOR_H_
#define _INDICOVECTOR_H_

#include "IndicoException.h"

#include <new>
#include <string.h>

namespace Indico
{
	template <class T> class Vector
	{
	private:
		unsigned int mCapacity;
		unsigned int mBlockSize;
		unsigned int mSize;
		T* mStorage;

	public:
		Vector() : mCapacity(8), mBlockSize(8), mSize(0), mStorage((T*)::operator new(sizeof(T)*8)) { }

		Vector(unsigned int blockSize) : mCapacity(blockSize), mBlockSize(blockSize), mSize(0), mStorage((T*)::operator new(sizeof(T)*blockSize))
		{
			if (!mBlockSize)
				throw Exception("Block size must be at least 1");
		}

		Vector(const Vector& src) : mCapacity(src.mCapacity), mBlockSize(src.mBlockSize), mSize(src.mSize), mStorage((T*)::operator new(sizeof(T)*mCapacity))
		{
			for (int i = -1; ++i < mSize;)::new (mStorage + i) T(src.mStorage[i]);
		}

		~Vector()
		{
			unsigned int start = 0;
			while (start < mSize)
				mStorage[start++].~T();

			::operator delete(mStorage);
		}

		unsigned int size() const
		{
			return mSize;
		}

		unsigned int capacity() const
		{
			return mCapacity;
		}

		unsigned int blockSize() const
		{
			return mBlockSize;
		}

		T& operator [] (unsigned int index)
		{
			if (index >= mSize)
				throw Exception("Index out of range");

			return mStorage[index];
		}

		const T& operator [] (unsigned int index) const
		{
			if (index >= mSize)
				throw Exception("Index out of range");

			return mStorage[index];
		}

		Vector& operator = (const Vector& other)
		{
			this->~Vector();

			mCapacity = other.mCapacity;
			mBlockSize = other.mBlockSize;
			mSize = other.mSize;
			mStorage = (T*)::operator new(sizeof(T)*mCapacity);

			for (int i = -1; ++i < mSize;)::new (mStorage + i) T(other.mStorage[i]);

			return *this;
		}

		void add(const T& item)
		{
			if (mSize == mCapacity)
			{
				T* storage = (T*)::operator new(sizeof(T)*(mCapacity+mBlockSize));
				memcpy(storage, mStorage, sizeof(T)*mSize);
				mCapacity += mBlockSize;

				::new (storage + mSize) T(item);
				mSize++;

				::operator delete(mStorage);
				mStorage = storage;
			}
			else
			{
				::new (mStorage + mSize) T(item);
				mSize++;
			}
		}

		void insert(const T& item, unsigned int index)
		{
			if (index >= mSize)
				throw Exception("Index out of range");

			unsigned int inc = mCapacity==mSize ? mBlockSize : 0;
			T* storage = (T*)::operator new(sizeof(T)*(mCapacity+inc));
			memcpy(storage, mStorage, sizeof(T)*index);
			memcpy(storage+index+1, mStorage+index, sizeof(T)*(mSize-index));
			mCapacity += inc;

			::new (storage + index) T(item);
			mSize++;

			::operator delete(mStorage);
			mStorage = storage;
		}

		void remove(unsigned int index)
		{
			if (index >= mSize)
				throw Exception("Index out of range");

			mStorage[index].~T();

			mCapacity = ((mSize-1) < (mCapacity-mBlockSize)) ? (mCapacity-mBlockSize) : mCapacity;

			T* storage = (T*)::operator new(sizeof(T)*mCapacity);

			memcpy(storage, mStorage, sizeof(T)*index);

			if (index < mSize - 1)
				memcpy(storage+index, mStorage+index+1, sizeof(T)*(mSize-(index+1)));

			mSize--;

			::operator delete(mStorage);
			mStorage = storage;
		}

		void clear()
		{
			for (unsigned int i = 0; i < mSize; i++)
			{
				mStorage[i].~T();
			}

			::operator delete(mStorage);
			mCapacity = 8;
			mBlockSize = 8;
			mSize = 0;
			mStorage = (T*)::operator new(sizeof(T)*8);
		}
	};
}

#endif
