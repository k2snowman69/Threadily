#include "ThreadQueueItem.h"

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <thread>
#endif

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
	
	ThreadQueueItem::ThreadQueueItem(std::function<void()> funcToRun)
	{
		this->funcToRun = funcToRun;
	}
	void ThreadQueueItem::run()
	{
		this->funcToRun();
	}
#ifdef EMSCRIPTEN
	//class IThreadQueueItemWrapper : public wrapper<IThreadQueueItem>
	struct IThreadQueueItemWrapper : public wrapper<IThreadQueueItem>
	{
		EMSCRIPTEN_WRAPPER(IThreadQueueItemWrapper);
		void run() {
			return call<void>("run");
		}
	};

	EMSCRIPTEN_BINDINGS(ThreadQueueItem) {
		class_<IThreadQueueItem>("IThreadQueueItem")
			.smart_ptr<std::shared_ptr<IThreadQueueItem>>("IThreadQueueItem")
			.function("run", &IThreadQueueItem::run, pure_virtual())
			.allow_subclass<IThreadQueueItemWrapper>("IThreadQueueItemWrapper")
			;
	}
#endif
}
