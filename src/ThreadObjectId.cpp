#include "ThreadObjectId.h"

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily 
{
#ifdef EMSCRIPTEN
	EMSCRIPTEN_BINDINGS(ThreadObjectId) {
		value_object<ThreadObjectId>("ThreadObjectId")
			.field("instanceId", &ThreadObjectId::instanceId)
			;
	}
#endif
}