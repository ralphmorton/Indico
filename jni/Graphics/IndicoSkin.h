
#ifndef _INDICOSKIN_H_
#define _INDICOSKIN_H_

#include "IndicoException.h"
#include "IndicoHashMap.h"
#include "IndicoResource.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"
#include "IndicoTypeInfo.h"
#include "Graphics/IndicoMaterial.h"

namespace Indico
{
	namespace Graphics
	{
		class Skin : public Resource
		{
		private:
			HashMap<string, Material*> mMaterials;

		public:
			Skin();

			bool has(const string& name) const;

			Material*& operator [](const string& name);
			const Material* operator [](const string& name) const;

			TYPED_DECL();
		};
	}
}

#endif
