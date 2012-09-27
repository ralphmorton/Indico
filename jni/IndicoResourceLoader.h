
#ifndef _INDICORESOURCELOADER_H_
#define _INDICORESOURCELOADER_H_

#include "IndicoResource.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"
#include "IndicoTypeInfo.h"

namespace Indico
{
	class ResourceLoader
	{
	public:
		virtual ~ResourceLoader() {}

		virtual shared_ptr<Resource> load(ResourceStream& stream) = 0;
		virtual const TypeInfo& loadType() const = 0;
	};
}

#endif
