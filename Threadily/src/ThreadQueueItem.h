#pragma once

#include <memory>
#include <functional>

#include "IThreadQueueItem.h"

namespace threadily {

	class ThreadQueueItem : public IThreadQueueItem {
	private:
		std::function<void()> funcToRun;
	public:
		ThreadQueueItem(std::function<void()> funcToRun);
		virtual void run();
	};

}