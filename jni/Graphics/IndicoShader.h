
#ifndef _INDICOSHADER_H_
#define _INDICOSHADER_H_

#include "IndicoException.h"
#include "IndicoResource.h"
#include "IndicoString.h"
#include "IndicoTypeInfo.h"
#include "Graphics/IndicoGL.h"

namespace Indico
{
	namespace Graphics
	{
		enum ShaderType
		{
			SHADERTYPE_VERTEX,
			SHADERTYPE_PIXEL
		};

		class Shader
		{
		private:
			ShaderType mType;
			GLuint mShader;

		private:
			Shader(const Shader& other) = delete;
			Shader& operator= (const Shader& other) = delete;

		public:
			Shader(ShaderType type, const string& source);
			virtual ~Shader();

			inline ShaderType type() const { return mType; }

			inline GLuint shader() const { return mShader; }
		};

		class VertexShader : public Shader, public Resource
		{
		private:
			VertexShader(const VertexShader& other) = delete;
			VertexShader& operator= (const VertexShader& other) = delete;

		public:
			VertexShader(const string& source);

			TYPED_DECL();
		};

		class PixelShader : public Shader, public Resource
		{
		private:
			PixelShader(const PixelShader& other) = delete;
			PixelShader& operator= (const PixelShader& other) = delete;

		public:
			PixelShader(const string& source);

			TYPED_DECL();
		};
	}
}

#endif
