
#include "IndicoEngine.h"
#include "IndicoAndroidResourceLocator.h"
#include "IndicoTimer.h"

namespace Indico
{
	template<> Engine* Singleton<Engine>::mSingleton = 0;

	Engine::Engine(App& app, struct android_app* androidApp) : mRender(androidApp), mApp(app), mAndroidApp(androidApp), mInited(false)
	{
		mAndroidApp->onAppCmd = Engine::handleCmd;

		shared_ptr<ResourceLocator> rloc((ResourceLocator*)new AndroidResourceLocator(mAndroidApp->activity->assetManager));
		mResMgr.addLocator(rloc);
	}

	Engine::~Engine()
	{

	}

	void Engine::handleCmd(struct android_app* androidApp, int32_t cmd)
	{
		Engine* e = Engine::getPtr();

		switch (cmd)
		{
		case APP_CMD_SAVE_STATE:
			break;
		case APP_CMD_INIT_WINDOW:
			e->mRender.init();
			e->mApp.init();
			e->mInited = true;
			break;
		case APP_CMD_TERM_WINDOW:
			break;
		case APP_CMD_LOST_FOCUS:
			break;
		case APP_CMD_GAINED_FOCUS:
			break;
		}
	}

	void Engine::run()
	{
		app_dummy();

		Timer timer;

		while (true)
		{
			int ident;
			int fdesc;
			int events;
			struct android_poll_source* source;

			while((ident = ALooper_pollAll(0, &fdesc, &events, (void**)&source)) >= 0)
			{
				if (source)
					source->process(mAndroidApp, source);
			}

			unsigned long long nanos = timer.nanos();
			timer.reset();

			if (mInited)
			{
				if (!mApp.update(nanos))
					break;

				mRender.frame();
			}
		}

		mApp.shutdown();
	}
}






