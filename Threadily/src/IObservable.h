#pragma once

#include "IThreadQueueItemManager.h"

namespace threadily {
	class ISubscribeHandle
	{
	};

	class IObservable
	{
	public:
		virtual std::shared_ptr<ISubscribeHandle> subscribe(std::shared_ptr<IThreadQueueItemManager> thread, std::shared_ptr<IObservable> other) = 0;
	};
}