#pragma once

#include <memory>
#include <vector>
#include "IObservable.h"
#include "IThreadObjectId.h"

namespace threadily {
	class IThreadObjectManager;
	class IThreadObject
	{
	public:
		virtual std::weak_ptr<IThreadObjectManager> getObjectManager() = 0;
		virtual unsigned int getThreadId() = 0;
		virtual const IThreadObjectId & getInstanceId() = 0;
		virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() = 0;
		virtual void runOnPeer(unsigned int threadId, std::function<void(std::shared_ptr<IThreadObject> peer)> toExecute) = 0;
		static std::shared_ptr<IThreadObject> getReference(std::shared_ptr<IThreadObject> value)
		{
			return value;
		};
	};

}
