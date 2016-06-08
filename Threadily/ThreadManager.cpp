#include "ThreadManager.h"

#include "ThreadQueueItemManager.h"

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <mutex>
#endif

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {

	std::shared_ptr<IThreadQueueItemManager> ThreadManager::getOrCreateThread(unsigned int threadId, std::vector<unsigned int> notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler> runner)
	{
		auto notifiesThreadIdsSet = std::set<unsigned int>(notifiesThreadIds.begin(), notifiesThreadIds.end());
		return this->getOrCreateThread(threadId, notifiesThreadIdsSet, nullptr);
	}
	std::shared_ptr<IThreadQueueItemManager> ThreadManager::getOrCreateThread(unsigned int threadId)
	{
		return this->getOrCreateThread(threadId, std::set<unsigned int>(), nullptr);
	}
	std::shared_ptr<IThreadQueueItemManager> ThreadManager::getOrCreateThread(unsigned int threadId, std::set<unsigned int>notifiesThreadIds)
	{
		return this->getOrCreateThread(threadId, notifiesThreadIds, nullptr);
	}
	std::shared_ptr<IThreadQueueItemManager> ThreadManager::getOrCreateThread(unsigned int threadId, std::set<unsigned int>notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler> runner)
	{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		// get a unique lock on the list of threadNameToObjects
		std::lock_guard<std::mutex> lock(threadNameToThreadHandler_m);
#endif

		// see if its in the list, if not create it and add it
		auto threadObjects_it = threadNameToThreadHandler.find(threadId);
		std::shared_ptr<IThreadQueueItemManager> threadHandler;
		if (threadObjects_it == threadNameToThreadHandler.end())
		{
			//std::static_pointer_cast<IThreadQueueItemManager>(
			threadHandler = std::make_shared<ThreadQueueItemManager>(this->shared_from_this(), threadId, notifiesThreadIds, runner);
			threadNameToThreadHandler.insert(std::pair<unsigned int, std::shared_ptr<IThreadQueueItemManager>>(threadId, threadHandler));
		}
		else
		{
			threadHandler = (*threadObjects_it).second;
		}

		// notify and return
		return threadHandler;
	}

	void ThreadManager::clear()
	{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		// get a unique lock on the list of threadNameToObjects
		std::lock_guard<std::mutex> lock(threadNameToThreadHandler_m);
#endif

		threadNameToThreadHandler.clear();
	}

#ifdef EMSCRIPTEN
	EMSCRIPTEN_BINDINGS(ThreadManager) {
		register_vector<unsigned int>("VectorUnsignedInt");
		class_<IThreadManager>("IThreadManager")
			.smart_ptr<std::shared_ptr<IThreadManager>>("IThreadManager")
			.function("clear", &IThreadManager::clear, pure_virtual())
			;
		class_<ThreadManager, base<IThreadManager>>("ThreadManager")
			.smart_ptr_constructor("ThreadManager", &std::make_shared<ThreadManager>)
			.function("getOrCreateThread", select_overload<std::shared_ptr<IThreadQueueItemManager>(unsigned int, std::vector<unsigned int>, std::shared_ptr<IThreadQueueItemHandler>)>(&ThreadManager::getOrCreateThread), allow_raw_pointers())
			.function("clear", &ThreadManager::clear)
			;
	}
#endif
}