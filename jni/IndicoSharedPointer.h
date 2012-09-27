
#ifndef _INDICOSHAREDPOINTER_H_
#define _INDICOSHAREDPOINTER_H_

namespace Indico
{
	template<class T> class shared_ptr
	{
	private:
		volatile int* mRefCount; //TODO: not std::atomic on android, use androids c-atomic stuff
		T** mItem;

	private:
		void dec()
		{
			if (--(*mRefCount) == 0)
			{
				delete *mItem;
				delete mItem;
				delete mRefCount;
				mItem = 0;
				mRefCount = 0;
			}
		}

	public:
		shared_ptr() : mRefCount(new int(1)), mItem(new T*(0)) { }
		shared_ptr(T* item) : mRefCount(new int(1)), mItem(new T*(item)) { }
		shared_ptr(const shared_ptr& other) : mRefCount(other.mRefCount), mItem(other.mItem)
		{
			(*mRefCount)++;
		}

		~shared_ptr()
		{
			dec();
		}

		T* get()
		{
			return *mItem;
		}

		const T* get() const
		{
			return *mItem;
		}

		T* operator -> ()
		{
			return *mItem;
		}

		const T* operator -> () const
		{
			return *mItem;
		}

		shared_ptr& operator = (const shared_ptr& other)
		{
			dec();

			(*other.mRefCount)++;
			mRefCount = other.mRefCount;
			mItem = other.mItem;

			return *this;
		}
	};
}


#endif
