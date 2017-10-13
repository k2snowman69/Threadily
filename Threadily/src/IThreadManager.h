#pragma once

#include <memory>

#include "IThreadQueueItemManager.h"
#include "IThreadQueueItemHandler.h"

namespace threadily {

	class IThreadManager
	{
	public:
		virtual std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId) = 0;
		virtual std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId, std::set<unsigned int>notifiesThreadIds) = 0;
		virtual std::shared_ptr<IThreadQueueItemManager> getOrCreateThread(unsigned int threadId, std::set<unsigned int>notifiesThreadIds, std::shared_ptr<IThreadQueueItemHandler>) = 0;
		virtual void clear() = 0;
	};

}