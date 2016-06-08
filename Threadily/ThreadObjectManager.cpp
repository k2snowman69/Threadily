#include "ThreadObjectManager.h"

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily
{
#ifdef EMSCRIPTEN
	EMSCRIPTEN_BINDINGS(ThreadObjectManager) {
	class_<IThreadObjectManager>("IThreadObjectManager")
		.smart_ptr<std::shared_ptr<IThreadObjectManager>>("IThreadObjectManager")
		.function("getThreadManager", &IThreadObjectManager::getThreadManager, pure_virtual())
		;
}
#endif
}