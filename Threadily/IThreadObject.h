#pragma once

#include <memory>
#include <vector>
#include "IObservable.h"

namespace threadily {
	class IThreadObjectManager;
	class IThreadObject
	{
	public:
		virtual std::weak_ptr<IThreadObjectManager> getObjectManager() = 0;
		virtual unsigned int getThreadId() = 0;
		virtual unsigned int getId() = 0;
		virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() = 0;
		virtual void runOnPeer(unsigned int threadId, std::function<void(std::shared_ptr<IThreadObject> peer)> toExecute) = 0;
	};

}
