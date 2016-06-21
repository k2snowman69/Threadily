#include "ParentThreadObject.h"

#include "ThreadObjectManager.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily
{
	namespace test
	{
		ParentThreadObject::ParentThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id) :
			ThreadObject(objectManager, threadId, id)
		{
			exampleObject = std::make_shared<Observable<ExampleThreadObject>>(threadId);
			auto threadObjectManager = std::make_shared<ThreadObjectManager<ExampleThreadObject>>(objectManager->getThreadManager());
			exampleObject->set(threadObjectManager->getOrCreateObject(threadId, 0));
		}

		std::vector<std::shared_ptr<IObservable>> ParentThreadObject::getObservableProperties()
		{
			auto d = std::vector<std::shared_ptr<IObservable>>();
			d.push_back(exampleObject);
			return d;
		}
#ifdef EMSCRIPTEN
		EMSCRIPTEN_BINDINGS(ParentThreadObjectManager) {
			class_<ParentThreadObject, base<ThreadObject>>("ParentThreadObject")
				.smart_ptr<std::shared_ptr<ParentThreadObject>>("ParentThreadObject")
				.property("name", &ParentThreadObject::name)
				;
			class_<ThreadObjectManager<ParentThreadObject>, base<IThreadObjectManager>>("ParentThreadObjectManager")
				.smart_ptr_constructor("ParentThreadObjectManager", &std::make_shared<ThreadObjectManager<ParentThreadObject>, std::shared_ptr<IThreadManager>>)
				//.smart_ptr<std::shared_ptr<ThreadObjectManager<ParentThreadObject>>>("ParentThreadObjectManager")
				.function("getOrCreateObject", &ThreadObjectManager<ParentThreadObject>::getOrCreateObject)
				;
		}
#endif
	}
}