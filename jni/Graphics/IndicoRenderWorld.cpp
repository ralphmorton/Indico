
#include "Graphics/IndicoRenderWorld.h"
#include "IndicoEngine.h"

namespace Indico
{
	namespace Graphics
	{
		RenderWorld::RenderWorld(struct android_app* androidApp) :
			mAndroidApp(androidApp),
			mDisplay(EGL_NO_DISPLAY),
			mSurface(EGL_NO_SURFACE),
			mContext(EGL_NO_CONTEXT),
			mWidth(0), mHeight(0),
			mProgram(0)
		{

		}

		RenderWorld::~RenderWorld()
		{
			if (mProgram)
				delete mProgram;

			if (mContext != EGL_NO_CONTEXT)
				eglDestroyContext(mDisplay, mContext);
			if (mSurface != EGL_NO_SURFACE)
				eglDestroySurface(mDisplay, mSurface);
			if (mDisplay != EGL_NO_DISPLAY)
				eglTerminate(mDisplay);
		}

		void RenderWorld::init()
		{
			const EGLint attribs[] =
			{
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_DEPTH_SIZE, 16,
				EGL_NONE
			};
			EGLint format;
			EGLint numConfigs;
			EGLConfig config;

			mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			eglInitialize(mDisplay, 0, 0);
			eglChooseConfig(mDisplay, attribs, &config, 1, &numConfigs);
			eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format);

			ANativeWindow_setBuffersGeometry(mAndroidApp->window, 0, 0, format);

			mSurface = eglCreateWindowSurface(mDisplay, config, mAndroidApp->window, NULL);

			EGLint c_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
			mContext = eglCreateContext(mDisplay, config, 0, c_attribs);

			if (eglMakeCurrent(mDisplay, mSurface, mSurface, mContext) == EGL_FALSE)
				throw Exception("eglMakeCurrent failed");

			eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth);
			eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight);

			mView = Matrix::createLookAt(Vector3::ONE*25, Vector3::ZERO, Vector3::UNIT_Y);
			mProj = Matrix::createPerspectiveFOV(Math::PI/3.0f, (float)mWidth/(float)mHeight, 1.0f, 1000.0f);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			shared_ptr<ResourceLoader> vloader(new VertexShaderLoader);
			shared_ptr<ResourceLoader> ploader(new PixelShaderLoader);
			shared_ptr<ResourceLoader> mloader(new MeshLoader);
			shared_ptr<ResourceLoader> tloader(new TextureLoader);
			shared_ptr<ResourceLoader> mtloader(new MaterialLoader);
			shared_ptr<ResourceLoader> skloader(new SkinLoader);

			ResourceManager& rman = Engine::getPtr()->getResourceManager();
			rman.addLoader(vloader);
			rman.addLoader(ploader);
			rman.addLoader(mloader);
			rman.addLoader(tloader);
			rman.addLoader(mtloader);
			rman.addLoader(skloader);

			VertexShader* vshader = rman.get<VertexShader>("test.vs");
			PixelShader* pshader = rman.get<PixelShader>("test.fs");

			mProgram = new Program;
			mProgram->attach(*vshader);
			mProgram->attach(*pshader);
			mProgram->link();
		}

		void RenderWorld::frame()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(mProgram->program());

			for (unsigned int i = 0; i < mModels.size(); i++)
			{
				Matrix mvp = mModels[i]->getTransform() * mView * mProj;
				mProgram->setMatrix("MVP", mvp);

				const Mesh& mesh = mModels[i]->mesh();
				const Skin& skin = mModels[i]->skin();

				glBindBuffer(GL_ARRAY_BUFFER, mesh.buf());

				const Graphics::VertexLayout& l = mesh.layout();

				for (unsigned int i = 0; i < l.numElements(); i++)
				{
					const Graphics::VertexElement& el = l[i];

					GLint num = 4;
					GLint type = GL_FLOAT;

					switch (el.type())
					{
					case Graphics::VERTEXELEMENT_FLOAT1:
						num = 1;
						break;
					case Graphics::VERTEXELEMENT_FLOAT2:
						num = 2;
						break;
					case Graphics::VERTEXELEMENT_FLOAT3:
						num = 3;
						break;
					case Graphics::VERTEXELEMENT_FLOAT4:
						num = 4;
						break;
					default:
						break;
					}

					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i, num, type, GL_FALSE, el.stride(), (void*)((int)el.offset()));
				}

				for (unsigned int s = 0; s < mesh.numSurfaces(); s++)
				{
					const Material* mat = skin[mesh[s].name()];

					mProgram->setTexture("tex", *mat->tex("DiffuseMap"), 0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh[s].buf());
					glDrawElements(GL_TRIANGLES, mesh[s].indexCount(), GL_UNSIGNED_INT, (void*)0);
				}

				for (unsigned int i = 0; i < l.numElements(); i++)
					glDisableVertexAttribArray(i);
			}

			eglSwapBuffers(mDisplay, mSurface);
		}

		void RenderWorld::addModel(Model* model)
		{
			mModels.add(model);
		}

		void RenderWorld::removeModel(Model* model)
		{
			for (unsigned int i = 0; i < mModels.size(); i++)
			{
				if (mModels[i] == model)
				{
					mModels.remove(i);
					return;
				}
			}
		}

		const Matrix& RenderWorld::getView() const
		{
			return mView;
		}

		const Matrix& RenderWorld::getProj() const
		{
			return mProj;
		}

		void RenderWorld::setView(const Matrix& view)
		{
			mView = view;
		}

		void RenderWorld::setProj(const Matrix& proj)
		{
			mProj = proj;
		}
	}
}
