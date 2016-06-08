#include "ExampleThreadObject.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily
{
	namespace test
	{
		ExampleThreadObject::ExampleThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id) :
			ThreadObject(objectManager, threadId, id)
		{
			name = std::make_shared<Observable<std::wstring>>();
		}

		std::vector<std::shared_ptr<IObservable>> ExampleThreadObject::getObservableProperties()
		{
			auto d = std::vector<std::shared_ptr<IObservable>>();
			d.push_back(name);
			return d;
		}
#ifdef EMSCRIPTEN
		EMSCRIPTEN_BINDINGS(ExampleThreadObjectManager) {
			class_<ExampleThreadObject, base<ThreadObject>>("ExampleThreadObject")
				.smart_ptr<std::shared_ptr<ExampleThreadObject>>("ExampleThreadObject")
				.property("name", &ExampleThreadObject::name)
				;
			class_<ThreadObjectManager<ExampleThreadObject>, base<IThreadObjectManager>>("ExampleThreadObjectManager")
				.smart_ptr_constructor("ExampleThreadObjectManager", &std::make_shared<ThreadObjectManager<ExampleThreadObject>, std::shared_ptr<IThreadManager>>)
				//.smart_ptr<std::shared_ptr<ThreadObjectManager<ExampleThreadObject>>>("ExampleThreadObjectManager")
				.function("getOrCreateObject", &ThreadObjectManager<ExampleThreadObject>::getOrCreateObject)
				;
		}
#endif
	}
}