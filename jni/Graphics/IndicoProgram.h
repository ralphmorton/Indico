
#ifndef _INDICOPROGRAM_H_
#define _INDICOPROGRAM_H_

#include "IndicoException.h"
#include "IndicoHashMap.h"
#include "IndicoMatrix.h"
#include "IndicoVector2.h"
#include "IndicoVector3.h"
#include "IndicoVector4.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoShader.h"
#include "Graphics/IndicoTexture.h"

namespace Indico
{
	namespace Graphics
	{
		class Program
		{
		private:
			GLuint mProgram;
			HashMap<string, GLuint> mParms;

		private:
			GLuint getParm(const string& name);

		public:
			Program();
			~Program();

			void attach(const Shader& shader);
			void detach(const Shader& shader);

			void link() const;

			void setInt(const string& name, int val);
			void setFloat(const string& name, float val);
			void setFloat2(const string& name, const Vector2& val);
			void setFloat3(const string& name, const Vector3& val);
			void setFloat4(const string& name, const Vector4& val);
			void setMatrix(const string& name, const Matrix& val);
			void setTexture(const string& name, const Texture& val, int index);

			inline GLuint program() const { return mProgram; }
		};
	}
}

#endif
