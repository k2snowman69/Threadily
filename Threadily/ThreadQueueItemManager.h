#pragma once

#include <queue>
#include <set>

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <mutex>
#endif

#include "IThreadManager.h"
#include "IThreadQueueItemHandler.h"
#include "IThreadQueueItem.h"

namespace threadily {

	/// Queues and runs all ThreadWork related to a specific thread
	class ThreadQueueItemManager : public IThreadQueueItemManager
	{
	private:
		std::set<unsigned int> notifiesThreadIds;
		std::shared_ptr<IThreadQueueItemHandler> threadDelegate;
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		std::mutex threadExecuting_m;
		std::mutex queuedJobs_m;
#endif
		std::weak_ptr<IThreadManager> threadManager;
		std::queue<std::shared_ptr<IThreadQueueItem>> queuedJobs;

		unsigned int threadId;
	public:
		ThreadQueueItemManager(std::weak_ptr<IThreadManager> threadManager, unsigned int threadName, std::set<unsigned int>notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler>);
		~ThreadQueueItemManager();
		virtual void addWork(std::shared_ptr<IThreadQueueItem>) override;
		virtual std::set<unsigned int> getThreadIdsThisNotifies() override;
		virtual bool isNotifiedBy(unsigned int threadId) override;
		virtual bool isNotifiedBy(std::shared_ptr<IThreadQueueItemManager>) override;
		virtual bool isNotifierFor(unsigned int threadId) override;
		virtual unsigned int getThreadId() override;
		void flushQueue();
	};

}
