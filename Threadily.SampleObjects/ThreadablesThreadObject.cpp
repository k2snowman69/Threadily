#include "ThreadablesThreadObject.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily
{
	namespace test
	{
		ThreadablesThreadObject::ThreadablesThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id) :
			ThreadObject(objectManager, threadId, id)
		{
			emptyObject = std::make_shared<Observable<EmptyThreadObject>>(threadId);
			emptyObjectArray = std::make_shared<Observable<std::vector<EmptyThreadObject>>>(threadId);
			emptyObject->set(nullptr);
		}

		std::vector<std::shared_ptr<IObservable>> ThreadablesThreadObject::getObservableProperties()
		{
			auto d = std::vector<std::shared_ptr<IObservable>>();
			d.push_back(emptyObject);
			d.push_back(emptyObjectArray);
			return d;
		}
#ifdef EMSCRIPTEN
		EMSCRIPTEN_BINDINGS(ThreadablesThreadObjectManager) {
			class_<ThreadablesThreadObject, base<ThreadObject<>>>("ThreadablesThreadObject")
				.smart_ptr<std::shared_ptr<ThreadablesThreadObject>>("ThreadablesThreadObject")
				.property("emptyObject", &ThreadablesThreadObject::emptyObject)
				.property("emptyObjectArray", &ThreadablesThreadObject::emptyObjectArray)
				;
			class_<ThreadObjectManager<ThreadablesThreadObject>, base<IThreadObjectManager>>("ThreadablesThreadObjectManager")
				.smart_ptr_constructor("ThreadablesThreadObjectManager", &std::make_shared<ThreadObjectManager<ThreadablesThreadObject>, std::shared_ptr<IThreadManager>>)
				.function("getOrCreateObject", &ThreadObjectManager<ThreadablesThreadObject>::getOrCreateObject)
				;
		}
#endif
	}
}