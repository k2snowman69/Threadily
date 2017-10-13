#pragma once

#include <set>
#include <memory>
#include "IThreadQueueItem.h"

namespace threadily {

	class IThreadQueueItemManager
	{
	public:
		virtual void addWork(std::shared_ptr<IThreadQueueItem>) = 0;
		virtual unsigned int getThreadId() = 0;
		virtual std::set<unsigned int> getThreadIdsThisNotifies() = 0;
		virtual bool isNotifiedBy(unsigned int threadId) = 0;
		virtual bool isNotifiedBy(std::shared_ptr<IThreadQueueItemManager>) = 0;
		virtual bool isNotifierFor(unsigned int threadId) = 0;
	};

}