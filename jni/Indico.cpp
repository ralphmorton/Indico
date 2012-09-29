
#include "IndicoApp.h"
#include "IndicoEngine.h"
#include "IndicoLog.h"
#include "IndicoMatrix.h"
#include "IndicoQuaternion.h"
#include "IndicoTransform.h"
#include "IndicoVector.h"
#include "IndicoVector3.h"
#include "IndicoResourceManager.h"
#include "Graphics/IndicoMesh.h"
#include "Graphics/IndicoSkin.h"
#include "Graphics/IndicoModel.h"
#include "Graphics/IndicoRenderWorld.h"
#include "Physics/IndicoBoxShape.h"
#include "Physics/IndicoCollisionObject.h"
#include "Physics/IndicoSimulator.h"
#include "Physics/IndicoRigidBody.h"
#include "Physics/IndicoSAP.h"

#include <android_native_app_glue.h>

using namespace Indico;

class TestApp : public App
{
private:
	Physics::SAP* mSAP;
	Physics::Simulator* mSim;
	Physics::BoxShape mBoxShape;
	Physics::CollisionObject* mFloor;
	Vector<Physics::RigidBody*> mBodies;
	Graphics::Model* mFloorModel;
	Vector<Graphics::Model*> mModels;
	float mRot;
	float mAccum;
	int mFrameC;

public:
	void init()
	{
		mRot = 0;
		mAccum = 0;
		mFrameC = 0;

		mSAP = new Physics::SAP(100);
		mSim = new Physics::Simulator(*mSAP);
		mSim->setGravity(-Vector3::UNIT_Y * 10.f);
		mSim->setCollisionBiasFactor(0.2f);
		mSim->setIterations(12);

		ResourceManager& rman = Engine::getPtr()->getResourceManager();
		Graphics::RenderWorld& renderer = Engine::getPtr()->getRenderWorld();

		Graphics::Mesh* mesh = rman.get<Graphics::Mesh>("cube.iam");
		Graphics::Skin* skin = rman.get<Graphics::Skin>("cube.ims");

		Transform ftrans(Quaternion::IDENTITY, Vector3(0,-1,0), Vector3(100,2,100));
		mFloor = new Physics::CollisionObject(ftrans, mBoxShape);
		mSim->addCollisionObject(mFloor);

		mFloorModel = new Graphics::Model(*mesh, *skin);
		mFloorModel->setTransform(ftrans.getMat());
		renderer.addModel(mFloorModel);

		renderer.setView(Matrix::createLookAt(Vector3::ONE*30, Vector3::UNIT_Y*6, Vector3::UNIT_Y));

		float y = 3.0f;
		for (int i = -3; ++i < 3;)
		{
			for (int j = -3; ++j < 3;)
			{
				Transform trans(Quaternion::IDENTITY, Vector3(i * 1.5f,y,j*-1.5f), Vector3(3,3,3));
				Physics::RigidBody* b = new Physics::RigidBody(mBoxShape, trans, 50.0f, 0.1f, 0.4f, 0.0f, 0.0f);
				mSim->addBody(b);

				Graphics::Model* m = new Graphics::Model(*mesh, *skin);
				m->setTransform(trans.getMat());
				renderer.addModel(m);

				mBodies.add(b);
				mModels.add(m);

				y += 4.5f;
			}
		}

		/*for (int i = 0; i < 6; i++)
		{
			Transform trans(Quaternion::IDENTITY, Vector3(0,1.5f + i*3.001f,0), Vector3(3,3,3));
			Physics::RigidBody* b = new Physics::RigidBody(mBoxShape, trans, 50.0f, 0.1f, 0.4f, 0.0f, 0.0f);
			mSim->addBody(b);

			Graphics::Model* m = new Graphics::Model(*mesh, *skin);
			m->setTransform(trans.getMat());
			renderer.addModel(m);

			mBodies.add(b);
			mModels.add(m);
		}*/
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

		mSim->step(secs*0.5f);

		for (int i = -1; ++i < mBodies.size();)
		{
			Matrix m = mBodies[(unsigned int)i]->getTransform().getMat();
			mModels[(unsigned int)i]->setTransform(m);
		}

		return true;
	}

	void shutdown()
	{
		mSim->clear();

		delete mFloorModel;

		for (int i = -1; ++i < mBodies.size();)
		{
			delete mBodies[(unsigned int)i];
			delete mModels[(unsigned int)i];
		}
	}
};

void android_main(struct android_app* androidApp)
{
	TestApp app;
	Engine* engine = new Engine(app, androidApp);
	engine->run();
}
