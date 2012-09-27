
#include "Graphics/IndicoTexture.h"

namespace Indico
{
	namespace Graphics
	{
		Texture::Texture(TextureWrap wrapS, TextureWrap wrapT, unsigned int width, unsigned int height, GLuint tex) : mWrapS(wrapS), mWrapT(wrapT), mWidth(width), mHeight(height), mTex(tex) { }
		Texture::~Texture() { glDeleteTextures(1, &mTex); }

		TYPED_DEF(Graphics::Texture);
	}
}
