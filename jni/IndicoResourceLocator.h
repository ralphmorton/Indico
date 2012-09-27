
#ifndef _INDICORESOURCELOCATOR_H_
#define _INDICORESOURCELOCATOR_H_

#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"

namespace Indico
{
	class ResourceLocator
	{
	public:
		virtual ~ResourceLocator() {}

		virtual bool exists(const string& name) = 0;
		virtual shared_ptr<ResourceStream> locate(const string& name) = 0;
	};
}

#endif
