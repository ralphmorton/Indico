
#ifndef _INDICORESOURCESTREAM_H_
#define _INDICORESOURCESTREAM_H_

#include "IndicoString.h"

namespace Indico
{
	class ResourceStream
	{
	public:
		virtual ~ResourceStream() {}

		virtual int length() const = 0;

		virtual int read(void* dest, unsigned int length) = 0;

		virtual bool eos() const = 0;

		virtual const string& getResourceID() const = 0;
	};
}

#endif
