
#ifndef _INDICOEXCEPTION_H_
#define _INDICOEXCEPTION_H_

#include "IndicoString.h"

namespace Indico
{
	class Exception
	{
	private:
		string mMessage;

	public:
		Exception(const string& message);
		~Exception();

		inline const string& message() const { return mMessage; }
	};
}

#endif
