
#include "IndicoApp.h"
#include "IndicoEngine.h"
#include "IndicoLog.h"
#include "IndicoMatrix.h"
#include "IndicoResourceManager.h"
#include "Graphics/IndicoMesh.h"
#include "Graphics/IndicoSkin.h"
#include "Graphics/IndicoModel.h"
#include "Graphics/IndicoRenderWorld.h"

#include <android_native_app_glue.h>

using namespace Indico;

class TestApp : public App
{
private:
	Graphics::Model* mModel;
	float mRot;
	float mAccum;
	int mFrameC;

public:
	void init()
	{
		mModel = 0;
		mRot = 0;
		mAccum = 0;
		mFrameC = 0;

		try
		{
			ResourceManager& rman = Engine::getPtr()->getResourceManager();
			Graphics::RenderWorld& renderer = Engine::getPtr()->getRenderWorld();

			Graphics::Mesh* mesh = rman.get<Graphics::Mesh>("cube.iam");
			Graphics::Skin* skin = rman.get<Graphics::Skin>("cube.ims");

			mModel = new Graphics::Model(*mesh, *skin);
			renderer.addModel(mModel);
		}
		catch (const Exception& ex)
		{
			LOGE("app", ex.message().c_str());
		}
	}

	bool update(unsigned long long nanos)
	{
		float secs = (float)(nanos/1000000000.0);
		mRot += secs;
		mAccum += secs;
		mFrameC++;

		if (mAccum >= 10.0f)
		{
			float fps = mFrameC / 10.0f;
			mAccum -= 10.0f;
			mFrameC = 0;

			LOGI("app", "%f fps", fps);
		}

		if (mModel)
		{
			Matrix m = Matrix::createScale(5,5,5) * Matrix::createRotationY(Math::PI * mRot * 0.5f);
			mModel->setTransform(m);
		}

		return true;
	}

	void shutdown()
	{
		delete mModel;
	}
};

void android_main(struct android_app* androidApp)
{
	TestApp app;
	Engine* engine = new Engine(app, androidApp);
	engine->run();
}
