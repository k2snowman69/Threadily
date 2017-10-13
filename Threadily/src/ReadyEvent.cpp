#include "ReadyEvent.h"

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <thread>
#endif

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
void ReadyEvent::finished()
{
	{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		std::lock_guard<std::mutex> lk(m);
#endif
		isFinished = true;
	}
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
	cv.notify_one();
#endif
}
void ReadyEvent::wait()
{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
	std::unique_lock<std::mutex> lk(m);
#endif
	if (isFinished == false)
	{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		std::thread t([this]()
#endif
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			std::unique_lock<std::mutex> lk(m);
			cv.wait(lk, [this]() { return isFinished; });
#endif
			isFinished = false;
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			lk.unlock();
#endif
		}
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		);
#endif

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		lk.unlock();
		t.join();
#endif
	}
	else
	{
		isFinished = false;
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		lk.unlock();
#endif
	}
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(ReadyEvent) {
	class_<ReadyEvent>("ReadyEvent")
		.smart_ptr_constructor("ReadyEvent", &std::make_shared<ReadyEvent>)
		.function("wait", &ReadyEvent::wait)
		.function("finished", &ReadyEvent::finished)
		;
}
#endif
}