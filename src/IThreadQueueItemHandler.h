#pragma once

#include <memory>

#include "IThreadQueueItem.h"

namespace threadily {

	/// Represents an object of which will run some work in a thread on behalf of the caller
	class IThreadQueueItemHandler {
	public:
		// A function which runs the specific ThreadWork. This is useful if you wish to bypass
		// the default thread functionality and implement your own thread mechanism
		virtual void run(std::shared_ptr<IThreadQueueItem>) = 0;
	};

}