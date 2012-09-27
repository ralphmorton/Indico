
#ifndef _INDICOSKINLOADER_H_
#define _INDICOSKINLOADER_H_

#include "IndicoLog.h"
#include "IndicoResource.h"
#include "IndicoResourceLoader.h"
#include "IndicoResourceManager.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoTypeInfo.h"
#include "IndicoVector.h"
#include "Graphics/IndicoMaterial.h"
#include "Graphics/IndicoSkin.h"

namespace Indico
{
	namespace Graphics
	{
		class SkinLoader : public ResourceLoader
		{
		public:
			shared_ptr<Resource> load(ResourceStream& stream);
			const TypeInfo& loadType() const;
		};
	}
}

#endif
