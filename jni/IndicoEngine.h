
#ifndef _INDICOENGINE_H_
#define _INDICOENGINE_H_

#include "IndicoAndroidResourceLocator.h"
#include "IndicoApp.h"
#include "IndicoException.h"
#include "IndicoLog.h"
#include "IndicoResourceManager.h"
#include "IndicoSingleton.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"
#include "Graphics/IndicoRenderWorld.h"

#include <android_native_app_glue.h>

namespace Indico
{
	class Engine : public Singleton<Engine>
	{
	private:
		ResourceManager mResMgr;
		Graphics::RenderWorld mRender;
		App& mApp;
		struct android_app* mAndroidApp;
		bool mInited;

	private:
		static void handleCmd(struct android_app* androidApp, int32_t cmd);

	public:
		Engine(App& app, struct android_app* androidApp);
		~Engine();

		void run();

		inline ResourceManager& getResourceManager() { return mResMgr; }
		inline Graphics::RenderWorld& getRenderWorld() { return mRender; }
	};
}

#endif
