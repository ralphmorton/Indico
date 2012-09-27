
#ifndef _INDICOSHADERLOADER_H_
#define _INDICOSHADERLOADER_H_

#include "IndicoResource.h"
#include "IndicoResourceLoader.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoTypeInfo.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoShader.h"

namespace Indico
{
	namespace Graphics
	{
		class VertexShaderLoader : public ResourceLoader
		{
		public:
			shared_ptr<Resource> load(ResourceStream& stream);
			const TypeInfo& loadType() const;
		};

		class PixelShaderLoader : public ResourceLoader
		{
		public:
			shared_ptr<Resource> load(ResourceStream& stream);
			const TypeInfo& loadType() const;
		};
	}
}

#endif
