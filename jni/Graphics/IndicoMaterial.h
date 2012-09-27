
#ifndef _INDICOMATERIAL_H_
#define _INDICOMATERIAL_H_

#include "IndicoException.h"
#include "IndicoHashMap.h"
#include "IndicoResource.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"
#include "IndicoTypeInfo.h"
#include "IndicoVector2.h"
#include "IndicoVector3.h"
#include "IndicoVector4.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoTexture.h"

namespace Indico
{
	namespace Graphics
	{
		class Material : public Resource
		{
		private:
			HashMap<string, Texture*> mTextures;
			HashMap<string, float> mFloats;
			HashMap<string, Vector2> mVec2s;
			HashMap<string, Vector3> mVec3s;
			HashMap<string, Vector4> mVec4s;

		public:
			Material();

			void set(const string& id, Texture* tex);
			void set(const string& id, float scalarf);
			void set(const string& id, const Vector2& vec);
			void set(const string& id, const Vector3& vec);
			void set(const string& id, const Vector4& vec);

			Texture* tex(const string& id) const;
			float scalarf(const string& id) const;
			const Vector2& vec2(const string& id) const;
			const Vector3& vec3(const string& id) const;
			const Vector4& vec4(const string& id) const;

			TYPED_DECL();
		};
	}
}

#endif
