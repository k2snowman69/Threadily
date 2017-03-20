#include "AppFactory.h"

#include <math.h>

#include <ThreadManager.h>
#include <ThreadObjectManager.h>

#include "AppManager.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
	namespace test {

#pragma region Factory
		AppFactory::AppFactory()
		{
			auto threadManager = std::make_shared<threadily::ThreadManager>();
			threadManager->getOrCreateThread(ThreadIds::ThreadId_UI); // UIThread
			threadManager->getOrCreateThread(ThreadIds::ThreadId_Service, { ThreadIds::ThreadId_UI }); // Service thread

			this->appThreadObjectManager = std::make_shared<AppManager>(threadManager);
			this->instanceCount = 0;
		}
		std::shared_ptr<App> AppFactory::create()
		{
			auto instanceCountCopy = instanceCount++;
			auto appObject = this->appThreadObjectManager->getOrCreateObject(ThreadIds::ThreadId_UI, instanceCountCopy);
			return appObject;
		}
#pragma endregion

#ifdef EMSCRIPTEN
		EMSCRIPTEN_BINDINGS(AppFactory)
		{
			class_<AppFactory>("AppFactory")
				.class_function("getInstance", &AppFactory::getInstance)
				.function("create", &AppFactory::create, allow_raw_pointers())
				;
		}
#endif
	}
}
