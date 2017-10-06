#pragma once

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <condition_variable>
#include <mutex>
#endif

namespace threadily {
class ReadyEvent
{
private:
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
	std::mutex m;
	std::condition_variable cv;
#endif
	bool isFinished;
public:
	ReadyEvent()
	{
		this->isFinished = false;
	};
	void finished();
	void wait();
};
}