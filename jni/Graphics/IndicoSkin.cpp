
#include "Graphics/IndicoSkin.h"

namespace Indico
{
	namespace Graphics
	{
		Skin::Skin() : mMaterials(0,5) { }

		bool Skin::has(const string& name) const
		{
			return mMaterials.has(name);
		}

		Material*& Skin::operator [](const string& name)
		{
			return mMaterials[name];
		}

		const Material* Skin::operator [](const string& name) const
		{
			return mMaterials[name];
		}

		TYPED_DEF(Graphics::Skin);
	}
}
