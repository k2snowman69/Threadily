#pragma once

#include <memory>

#include "IThreadQueueItemHandler.h"

#include "ThreadQueueItem.h"

namespace threadily {

	/// A default representation of a thread runner that runs everything in a separate std::thread
	class ThreadQueueItemHandler : public IThreadQueueItemHandler {
	public:
		virtual void run(std::shared_ptr<IThreadQueueItem>);
	};

}