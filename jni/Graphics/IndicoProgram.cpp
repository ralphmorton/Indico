
#include "Graphics/IndicoProgram.h"

namespace Indico
{
	namespace Graphics
	{
		Program::Program() : mParms(0,10)
		{
			mProgram = glCreateProgram();

			if (!mProgram)
				throw Exception("Failed to create program");
		}

		Program::~Program()
		{
			glDeleteProgram(mProgram);
		}

		GLuint Program::getParm(const string& name) //TODO: again, need documentation to know what happens on error
		{
			GLuint id = 0;

			if (!mParms.has(name))
			{
				id = glGetUniformLocation(mProgram, name.c_str());
				mParms[name] = id;
			}
			else
				id = mParms[name];

			return id;
		}

		void Program::attach(const Shader& shader)
		{
			glAttachShader(mProgram, shader.shader());

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				throw Exception("Could not detach the specified shader");
		}

		void Program::detach(const Shader& shader)
		{
			glDetachShader(mProgram, shader.shader());

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				throw Exception("Could not detach the specified shader");
		}

		void Program::link() const
		{
			glLinkProgram(mProgram);

			GLint status;
			glGetProgramiv(mProgram, GL_LINK_STATUS, &status);

			if (!status)
				throw Exception("Could not link program"); //TODO: error details
		}

		void Program::setInt(const string& name, int val)
		{
			GLuint id = getParm(name);
			glUniform1i(id, val);
		}

		void Program::setFloat(const string& name, float val)
		{
			GLuint id = getParm(name);
			glUniform1f(id, val);
		}

		void Program::setFloat2(const string& name, const Vector2& val)
		{
			GLuint id = getParm(name);
			glUniform2f(id, val.x, val.y);
		}

		void Program::setFloat3(const string& name, const Vector3& val)
		{
			GLuint id = getParm(name);
			glUniform3f(id, val.x, val.y, val.z);
		}

		void Program::setFloat4(const string& name, const Vector4& val)
		{
			GLuint id = getParm(name);
			glUniform4f(id, val.x, val.y, val.z, val.w);
		}

		void Program::setMatrix(const string& name, const Matrix& val)
		{
			GLuint id = getParm(name);

			glUniformMatrix4fv(id, 1, GL_FALSE, val._m);
		}

		void Program::setTexture(const string& name, const Texture& val, int index)
		{
			GLuint id = getParm(name);

			glActiveTexture(GL_TEXTURE0+index);
			glBindTexture(GL_TEXTURE_2D, val.tex());
			glUniform1i(id, index);
		}
	}
}
















