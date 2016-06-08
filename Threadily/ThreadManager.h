#pragma once

#include <mutex>
#include <map>
#include <set>
#include <vector>

#include "IThreadManager.h"
#include "IThreadQueueItemHandler.h"

namespace threadily {

	class ThreadManager : public IThreadManager, public std::enable_shared_from_this<ThreadManager>
	{
	public:
		ThreadManager() {};

	private:
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		std::mutex threadNameToThreadHandler_m;
#endif
		std::map<unsigned int, std::shared_ptr<IThreadQueueItemManager>> threadNameToThreadHandler;

	public:
		std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId);
		std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId, std::set<unsigned int>notifiesThreadIds);
		std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId, std::set<unsigned int>notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler>);
		std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId, std::vector<unsigned int> notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler> runner);
		void clear();
	};

}
