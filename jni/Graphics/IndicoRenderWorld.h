
#ifndef _INDICORENDERWORLD_H_
#define _INDICORENDERWORLD_H_

#include "IndicoLog.h"
#include "IndicoMath.h"
#include "IndicoMatrix.h"
#include "IndicoVector.h"
#include "IndicoVector3.h"
#include "Graphics/IndicoGL.h"
#include "Graphics/IndicoMaterial.h"
#include "Graphics/IndicoMaterialLoader.h"
#include "Graphics/IndicoMesh.h"
#include "Graphics/IndicoMeshLoader.h"
#include "Graphics/IndicoModel.h"
#include "Graphics/IndicoProgram.h"
#include "Graphics/IndicoShaderLoader.h"
#include "Graphics/IndicoSkin.h"
#include "Graphics/IndicoSkinLoader.h"
#include "Graphics/IndicoTexture.h"
#include "Graphics/IndicoTextureLoader.h"
#include "Graphics/IndicoVertexLayout.h"

#include <android_native_app_glue.h>

namespace Indico
{
	namespace Graphics
	{
		class RenderWorld
		{
		private:
			struct android_app* mAndroidApp;
			EGLDisplay mDisplay;
			EGLSurface mSurface;
			EGLContext mContext;
			int mWidth;
			int mHeight;
			Matrix mView, mProj;
			Vector<Model*> mModels;

			Program* mProgram;

		public:
			RenderWorld(struct android_app* androidApp);
			~RenderWorld();

			void init();

			void frame();

			void addModel(Model* model);
			void removeModel(Model* model);

			const Matrix& getView() const;
			const Matrix& getProj() const;

			void setView(const Matrix& view);
			void setProj(const Matrix& proj);
		};
	}
}

#endif
