
#ifndef _INDICOTEXTURE_H_
#define _INDICOTEXTURE_H_

#include "IndicoException.h"
#include "IndicoResource.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"
#include "IndicoTypeInfo.h"
#include "Graphics/IndicoGL.h"

namespace Indico
{
	namespace Graphics
	{
		enum TextureWrap
		{
			WRAP_CLAMP = 0x2900, //GL_CLAMP
			WRAP_CLAMPEDGE = 0x812F, //GL_CLAMP_TO_EDGE
			WRAP_REPEAT = 0x2901 //GL_REPEAT
		};

		class Texture : public Resource
		{
		private:
			friend class TextureLoader;

		private:
			TextureWrap mWrapS;
			TextureWrap mWrapT;
			unsigned int mWidth;
			unsigned int mHeight;
			GLuint mTex;

		private:
			Texture(const Texture& other) = delete;
			Texture& operator = (const Texture& other) = delete;

		protected:
			Texture(TextureWrap wrapS, TextureWrap wrapT, unsigned int width, unsigned int height, GLuint tex);

		public:
			~Texture();

			inline TextureWrap wrapS() const { return mWrapS; }
			inline TextureWrap wrapT() const { return mWrapT; }
			inline unsigned int width() const { return mWidth; }
			inline unsigned int height() const { return mHeight; }
			inline GLuint tex() const { return mTex; }

			TYPED_DECL();
		};
	}
}

#endif
