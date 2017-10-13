#include "ThreadQueueItemManager.h"

#include "ThreadQueueItemHandler.h"

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
ThreadQueueItemManager::ThreadQueueItemManager(std::weak_ptr<IThreadManager> threadManager, unsigned int threadId, std::set<unsigned int>notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler> runner)
{
	this->threadManager = threadManager;
	this->threadId = threadId;
	this->threadDelegate = runner;
	this->notifiesThreadIds = notifiesThreadIds;
}

ThreadQueueItemManager::~ThreadQueueItemManager()
{
	this->threadDelegate = nullptr;
}

std::set<unsigned int> ThreadQueueItemManager::getThreadIdsThisNotifies()
{
	return this->notifiesThreadIds;
}

bool ThreadQueueItemManager::isNotifiedBy(unsigned int threadId)
{
	auto threadManagerStrong = this->threadManager.lock();
	if (threadManagerStrong != nullptr)
	{
		return this->isNotifiedBy(threadManagerStrong->getOrCreateThread(threadId));
	}
	else
	{
		return false;
	}
}

bool ThreadQueueItemManager::isNotifiedBy(std::shared_ptr<IThreadQueueItemManager> threadQueueManager)
{
	return threadQueueManager->isNotifierFor(this->getThreadId());
}

bool ThreadQueueItemManager::isNotifierFor(unsigned int threadId)
{
	return this->notifiesThreadIds.find(threadId) != this->notifiesThreadIds.end();
}

void ThreadQueueItemManager::addWork(std::shared_ptr<IThreadQueueItem> work)
{
	{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		// get a unique lock on the queue of jobs
		std::lock_guard<std::mutex> lock(queuedJobs_m);
#endif

		// add the job to the queue
		queuedJobs.push(work);
	}

	if (threadDelegate == nullptr)
	{
		threadDelegate = std::make_shared<ThreadQueueItemHandler>();
	}
	threadDelegate->run(work);
}

unsigned int ThreadQueueItemManager::getThreadId()
{
	return this->threadId;
}

void ThreadQueueItemManager::flushQueue()
{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
	// if the queue isn't running, start executing
	if (this->threadExecuting_m.try_lock())
#endif
	{

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		// get a unique lock on the queue of jobs
		queuedJobs_m.lock();
#endif

		// add the job to the queue
		while (!queuedJobs.empty())
		{
			// get and remove the first element in the queue
			auto work = queuedJobs.front();
			queuedJobs.pop();

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// release the lock ont the queue of jobs
			queuedJobs_m.unlock();
#endif

			// execute the work
			work->run();

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// get a unique lock on the queue of jobs
			queuedJobs_m.lock();
#endif
		}

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		// unlock the thread since it's done
		queuedJobs_m.unlock();
#endif
	}

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
	this->threadExecuting_m.unlock();
#endif
}

#ifdef EMSCRIPTEN
	EMSCRIPTEN_BINDINGS(ThreadQueueItemManager) {
		class_<IThreadQueueItemManager>("IThreadQueueItemManager")
			.smart_ptr<std::shared_ptr<IThreadQueueItemManager>>("IThreadQueueItemManager")
			.function("getThreadId", &IThreadQueueItemManager::getThreadId, pure_virtual())
			;
		class_<ThreadQueueItemManager>("ThreadQueueItemManager")
			.function("getThreadId", &ThreadQueueItemManager::getThreadId)
			;
	}
#endif
}
