#include "ThreadObject.h"

#include "ReadyEvent.h"
#include "ThreadQueueItem.h"

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily 
{
#ifdef EMSCRIPTEN
	EMSCRIPTEN_BINDINGS(ThreadObject) {
		class_<IThreadObject>("IThreadObject")
			.smart_ptr<std::shared_ptr<IThreadObject>>("IThreadObject")
			.function("getThreadId", &IThreadObject::getThreadId, pure_virtual())
			//.function("getId", &IThreadObject::getInstanceId, pure_virtual())
			;
	}
#endif
}