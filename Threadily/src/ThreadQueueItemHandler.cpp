#include "ThreadQueueItemHandler.h"

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <thread>
#endif

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {

	void ThreadQueueItemHandler::run(std::shared_ptr<IThreadQueueItem> work)
	{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		std::thread t([this, work]()
#endif
		{
			work->run();
		}
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		);
		t.detach();
#endif
	}

#ifdef EMSCRIPTEN
	class IThreadQueueItemHandlerWrapper : public wrapper<IThreadQueueItemHandler>
	{
	public:
		EMSCRIPTEN_WRAPPER(IThreadQueueItemHandlerWrapper);
		void run(std::shared_ptr<IThreadQueueItem> work) {
			return call<void>("run", work);
		}
	};

	EMSCRIPTEN_BINDINGS(ThreadQueueItemHandler) {
		class_<IThreadQueueItemHandler>("IThreadQueueItemHandler")
			.smart_ptr<std::shared_ptr<IThreadQueueItemHandler>>("IThreadQueueItemHandler")
			.function("run", &IThreadQueueItemHandler::run, pure_virtual())
			.allow_subclass<IThreadQueueItemHandlerWrapper>("IThreadQueueItemHandlerWrapper")
			;
	}
#endif
}
