#include "PrimativesThreadObject.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily
{
	namespace test
	{
		PrimativesThreadObject::PrimativesThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id) :
			ThreadObject(objectManager, threadId, id)
		{
			name = std::make_shared<Observable<std::wstring>>();
			intValue = std::make_shared<Observable<int>>();
			stringArray = std::make_shared<Observable<std::vector<std::wstring>>>();
			intArray = std::make_shared<Observable<std::vector<int>>>();
		}

		std::vector<std::shared_ptr<IObservable>> PrimativesThreadObject::getObservableProperties()
		{
			auto d = std::vector<std::shared_ptr<IObservable>>();
			d.push_back(name);
			d.push_back(intValue);
			d.push_back(stringArray);
			d.push_back(intArray);
			return d;
		}
#ifdef EMSCRIPTEN
		EMSCRIPTEN_BINDINGS(PrimativesThreadObjectManager) {
			class_<PrimativesThreadObject, base<ThreadObject<>>>("PrimativesThreadObject")
				.smart_ptr<std::shared_ptr<PrimativesThreadObject>>("PrimativesThreadObject")
				.property("intValue", &PrimativesThreadObject::intValue)
				.property("name", &PrimativesThreadObject::name)
				.property("stringArray", &PrimativesThreadObject::stringArray)
				.property("intArray", &PrimativesThreadObject::intArray)
				;
			class_<ThreadObjectManager<PrimativesThreadObject>, base<IThreadObjectManager>>("PrimativesThreadObjectManager")
				.smart_ptr_constructor("PrimativesThreadObjectManager", &std::make_shared<ThreadObjectManager<PrimativesThreadObject>, std::shared_ptr<IThreadManager>>)
				.function("getOrCreateObject", &ThreadObjectManager<PrimativesThreadObject>::getOrCreateObject)
				;
		}
#endif
	}
}