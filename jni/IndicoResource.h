
#ifndef _INDICORESOURCE_H_
#define _INDICORESOURCE_H_

#include "IndicoString.h"
#include "IndicoTypeInfo.h"

namespace Indico
{
	class Resource
	{
	public:
		virtual ~Resource();

		TYPED_DECL();
	};
}

#endif
