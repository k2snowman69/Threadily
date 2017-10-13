#include "EmptyThreadObject.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily
{
	namespace test
	{
		EmptyThreadObject::EmptyThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id) :
			ThreadObject(objectManager, threadId, id)
		{
			
		}

		std::vector<std::shared_ptr<IObservable>> EmptyThreadObject::getObservableProperties()
		{
			auto d = std::vector<std::shared_ptr<IObservable>>();
			return d;
		}

		// Need to locally define getThreadId so that embind can actually bind to it... annoyances.
		unsigned int EmptyThreadObject::getThreadId()
		{
			return ThreadObject::getThreadId();
		}

#ifdef EMSCRIPTEN

		defineThreadObjectCallbackHandler(EmptyThreadObject, EmptyThreadObject);
		defineThreadObjectCallbackVectorHandler(EmptyThreadObject, EmptyThreadObject);

		EMSCRIPTEN_BINDINGS(EmptyThreadObjectManager) {
			defineThreadObjectObservable(EmptyThreadObject, EmptyThreadObject);
			defineThreadObjectObservableVector(EmptyThreadObject, EmptyThreadObject);

			class_<EmptyThreadObject, base<ThreadObject<>>>("EmptyThreadObject")
				.smart_ptr<std::shared_ptr<EmptyThreadObject>>("EmptyThreadObject")
				.function("getThreadId", &EmptyThreadObject::getThreadId)
				;
			class_<ThreadObjectManager<EmptyThreadObject>, base<IThreadObjectManager>>("EmptyThreadObjectManager")
				.smart_ptr_constructor("EmptyThreadObjectManager", &std::make_shared<ThreadObjectManager<EmptyThreadObject>, std::shared_ptr<IThreadManager>>)
				.function("getOrCreateObject", &ThreadObjectManager<EmptyThreadObject>::getOrCreateObject)
				;
		}
#endif
	}
}