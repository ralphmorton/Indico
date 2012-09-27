
#ifndef _INDICOSINGLETON_H_
#define _INDICOSINGLETON_H_

#include <assert.h>

namespace Indico
{
	template <typename T> class Singleton
    {
    protected:
        static T* mSingleton;

	private:
		Singleton(const Singleton<T> &);
		Singleton& operator=(const Singleton<T> &);

    public:
        Singleton()
        {
            assert(!mSingleton);
			mSingleton = static_cast<T*>(this);
        }

        ~Singleton()
        {
			assert(mSingleton);
			mSingleton = 0;
		}

        static T& get()
		{
			assert(mSingleton);
			return (*mSingleton);
		}

        static T* getPtr()
		{
			return mSingleton;
		}
    };
}

#endif
