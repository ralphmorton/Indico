
#ifndef _INDICOMESHLOADER_H_
#define _INDICOMESHLOADER_H_

#include "IndicoLog.h"
#include "IndicoResource.h"
#include "IndicoResourceLoader.h"
#include "IndicoResourceManager.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoTypeInfo.h"
#include "IndicoVector.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoMesh.h"
#include "Graphics/IndicoVertexLayout.h"

namespace Indico
{
	namespace Graphics
	{
		class MeshLoader : public ResourceLoader
		{
		private:
			void readVertexElement(const string& el, Vector<VertexElementType>& types, Vector<unsigned short>& offsets, unsigned short& numf) const;
			unsigned int readVertexLayout(const string& line, VertexLayout& layout) const;

		public:
			shared_ptr<Resource> load(ResourceStream& stream);
			const TypeInfo& loadType() const;
		};
	}
}

#endif
