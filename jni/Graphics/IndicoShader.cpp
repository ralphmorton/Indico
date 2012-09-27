
#include "Graphics/IndicoShader.h"

namespace Indico
{
	namespace Graphics
	{
		Shader::Shader(ShaderType type, const string& source) : mType(type)
		{
			int t = 0;
			switch (type)
			{
			case SHADERTYPE_VERTEX:
				t = GL_VERTEX_SHADER;
				break;
			case SHADERTYPE_PIXEL:
				t = GL_FRAGMENT_SHADER;
				break;
			default:
				throw Exception("Unknown shader type");
			}

			const char* src = source.c_str();
			int len = source.length();

			mShader = glCreateShader(t);

			if (!mShader)
				throw Exception("Failed to create shader");

			glShaderSource(mShader, 1, &src, &len);
			glCompileShader(mShader);

			int compiled;
			glGetShaderiv(mShader, GL_COMPILE_STATUS, &compiled);

			if (!compiled)
				throw Exception("Failed to compile shader"); //TODO: info from glGetShaderInfoLog
		}

		Shader::~Shader()
		{
			glDeleteShader(mShader);
		}

		VertexShader::VertexShader(const string& source) : Shader(SHADERTYPE_VERTEX, source) { }
		PixelShader::PixelShader(const string& source) : Shader(SHADERTYPE_PIXEL, source) { }

		TYPED_DEF(Graphics::VertexShader);
		TYPED_DEF(Graphics::PixelShader);
	}
}




