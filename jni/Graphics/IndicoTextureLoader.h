
#ifndef _INDICOTEXTURELOADER_H_
#define _INDICOTEXTURELOADER_H_

#include "IndicoLog.h"
#include "IndicoResource.h"
#include "IndicoResourceLoader.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoTypeInfo.h"
#include "IndicoVector.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoTexture.h"

namespace Indico
{
	namespace Graphics
	{
		class TextureLoader : public ResourceLoader
		{
		public:
			shared_ptr<Resource> load(ResourceStream& stream);
			const TypeInfo& loadType() const;
		};
	};
}

#endif
