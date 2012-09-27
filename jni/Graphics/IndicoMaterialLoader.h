
#ifndef _INDICOMATERIALLOADER_H_
#define _INDICOMATERIALLOADER_H_

#include "IndicoResource.h"
#include "IndicoResourceLoader.h"
#include "IndicoResourceManager.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoTypeInfo.h"
#include "IndicoVector.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoMaterial.h"
#include "Graphics/IndicoTexture.h"

namespace Indico
{
	namespace Graphics
	{
		class MaterialLoader : public ResourceLoader
		{
		public:
			shared_ptr<Resource> load(ResourceStream& stream);
			const TypeInfo& loadType() const;
		};
	}
}

#endif
