#pragma once

#include <mutex>
#include <map>
#include <set>
#include <vector>

#include "IThreadObjectManager.h"
#include "IThreadObject.h"

namespace threadily {

	class ThreadObject : public IThreadObject , public std::enable_shared_from_this<ThreadObject>
	{
	public:
		ThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);
		~ThreadObject();

	private:
		// this object needs to hold onto a reference to it's peer objects it will need to notify
		// This it to solve the issue of Service -> App -> UI where only the service and UI objects
		// have been created
		std::shared_ptr<std::set<std::shared_ptr<ThreadObject>>> peerObjectsToNotify;
		std::shared_ptr<std::vector<std::weak_ptr<ThreadObject>>> disposeListeners;
		std::shared_ptr<std::map<unsigned int, std::shared_ptr<std::vector<std::shared_ptr<ISubscribeHandle>>>>> threadObjectToChangeSubscribers;

		unsigned int threadId;
		unsigned int id;
		std::weak_ptr<IThreadObjectManager> objectManager;
		void addDisposeListener(std::shared_ptr<ThreadObject> thingToNotify);
	public:
		void addPeerObjectToNotify(std::shared_ptr<ThreadObject>);
		void initLinks(std::shared_ptr<ThreadObject> baseObject);
		virtual std::shared_ptr<IThreadObjectManager> getObjectManager() override;
		virtual unsigned int getThreadId() override;
		virtual unsigned int getId() override;
		virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
	};

}
