
#include "Graphics/IndicoMaterial.h"

namespace Indico
{
	namespace Graphics
	{
		Material::Material() : mTextures(0,5), mFloats(0,5), mVec2s(0,5), mVec3s(0,5), mVec4s(0,5) { }

		void Material::set(const string& id, Texture* tex) { mTextures[id] = tex; }
		void Material::set(const string& id, float scalarf) { mFloats[id] = scalarf; }
		void Material::set(const string& id, const Vector2& vec) { mVec2s[id] = vec; }
		void Material::set(const string& id, const Vector3& vec) { mVec3s[id] = vec; }
		void Material::set(const string& id, const Vector4& vec) { mVec4s[id] = vec; }

		Texture* Material::tex(const string& id) const { return mTextures[id]; }
		float Material::scalarf(const string& id) const { return mFloats[id]; }
		const Vector2& Material::vec2(const string& id) const { return mVec2s[id]; }
		const Vector3& Material::vec3(const string& id) const { return mVec3s[id]; }
		const Vector4& Material::vec4(const string& id) const { return mVec4s[id]; }

		TYPED_DEF(Graphics::Material);
	}
}
