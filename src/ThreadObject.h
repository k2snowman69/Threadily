#pragma once

#include <mutex>
#include <map>
#include <set>
#include <vector>

#include "IThreadObjectManager.h"
#include "IThreadObject.h"
#include "ThreadObjectId.h"
#include "ReadyEvent.h"
#include "IThreadQueueItem.h"
#include "ThreadQueueItem.h"

namespace threadily {

	template<typename T_Id = ThreadObjectId>
	class ThreadObject : public IThreadObject, public std::enable_shared_from_this<ThreadObject<T_Id>>
	{
	private /*variables*/:
		// this object needs to hold onto a reference to it's peer objects it will need to notify
		// This it to solve the issue of Service -> App -> UI where only the service and UI objects
		// have been created
		std::shared_ptr<std::set<std::shared_ptr<ThreadObject<T_Id>>>> peerObjectsToNotify;
		std::shared_ptr<std::vector<std::weak_ptr<ThreadObject<T_Id>>>> disposeListeners;
		std::shared_ptr<std::map<unsigned int, std::shared_ptr<std::vector<std::shared_ptr<ISubscribeHandle>>>>> threadObjectToChangeSubscribers;

		unsigned int threadId;
		T_Id id;
		std::weak_ptr<IThreadObjectManager> objectManager;
	private /*methods*/:
		void addDisposeListener(std::shared_ptr<ThreadObject<T_Id>> thingToNotify)
		{
			this->disposeListeners->push_back(thingToNotify);
		}
	public /*methods*/:
		ThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, const T_Id & id)
		{
			if (objectManager == nullptr)
			{
				throw std::runtime_error("ObjectManager must be defined");
			}
			this->objectManager = objectManager;
			this->threadId = threadId;
			this->id = id;

			this->peerObjectsToNotify = std::make_shared<std::set<std::shared_ptr<ThreadObject<T_Id>>>>();
			this->disposeListeners = std::make_shared<std::vector<std::weak_ptr<ThreadObject<T_Id>>>>();
			this->threadObjectToChangeSubscribers = std::make_shared<std::map<unsigned int, std::shared_ptr<std::vector<std::shared_ptr<ISubscribeHandle>>>>>();
		}
		~ThreadObject()
		{
			auto objectManagerStrong = this->objectManager.lock();
			if (objectManagerStrong != nullptr)
			{
				auto threadManager = objectManagerStrong->getThreadManager();

				for (auto it = this->disposeListeners->begin(); it != this->disposeListeners->end(); ++it)
				{
					std::weak_ptr<ThreadObject<T_Id>> listenerWeak = *it;
					if (!listenerWeak.expired())
					{
						std::shared_ptr<ThreadObject<T_Id>> listener = listenerWeak.lock();
						ReadyEvent re;
						auto listenerObjThread = threadManager->getOrCreateThread(listener->getThreadId());
						listenerObjThread->addWork(std::make_shared<ThreadQueueItem>([&re, this, listener]() {
							listener->threadObjectToChangeSubscribers->erase(this->getThreadId());
							re.finished();
						}));
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
						re.wait();
#endif
					}
				}
			}

			this->peerObjectsToNotify->clear();
			this->disposeListeners->clear();
			this->threadObjectToChangeSubscribers->clear();
		}
		void addPeerObjectToNotify(std::shared_ptr<ThreadObject<T_Id>> peerObject)
		{
			this->peerObjectsToNotify->insert(peerObject);
		}
		void initLinks(std::shared_ptr<ThreadObject<T_Id>> baseObject)
		{
			auto objectManagerStrong = this->objectManager.lock();
			if (objectManagerStrong == nullptr)
			{
				throw std::runtime_error("Object manager is null while attempting to initialize links between two objects");
			}
			auto threadManager = objectManagerStrong->getThreadManager();
			auto thisObjThread = threadManager->getOrCreateThread(this->getThreadId());
			auto baseObjThread = threadManager->getOrCreateThread(baseObject->getThreadId());
			if (!thisObjThread->isNotifiedBy(baseObjThread))
			{
				return;
			}

			// get or insert the element in the subscribers dictionary
			auto changeSubscribersItr = threadObjectToChangeSubscribers->find(baseObject->getThreadId());
			if (changeSubscribersItr == threadObjectToChangeSubscribers->end())
			{
				threadObjectToChangeSubscribers->insert(std::pair<unsigned int, std::shared_ptr<std::vector<std::shared_ptr<ISubscribeHandle>>>>(baseObject->getThreadId(), std::make_shared<std::vector<std::shared_ptr<ISubscribeHandle>>>()));
				changeSubscribersItr = threadObjectToChangeSubscribers->find(baseObject->getThreadId());
			}
			std::shared_ptr<std::vector<std::shared_ptr<ISubscribeHandle>>> changeSubscribers = changeSubscribersItr->second;

			// Iterate through all the observables and start notification between
			// the base object and this object
			std::vector<std::shared_ptr<IObservable>> thisObservables = this->getObservableProperties();
			std::vector<std::shared_ptr<IObservable>> baseObservables = baseObject->getObservableProperties();
			if (thisObservables.size() != baseObservables.size())
			{
				throw std::runtime_error("Expected obsevableProperties to always be the same size");
			}
			auto thisIt = thisObservables.begin();
			auto baseIt = baseObservables.begin();
			for (; thisIt != thisObservables.end(); ++thisIt, ++baseIt)
			{
				std::shared_ptr<IObservable> thisObservable = *thisIt;
				std::shared_ptr<IObservable> baseObservable = *baseIt;
				std::shared_ptr<ISubscribeHandle> subscription = baseObservable->subscribe(threadManager->getOrCreateThread(this->getThreadId()), thisObservable);
				changeSubscribers->push_back(subscription);
			}

			// add a dispose listener to baseObject so that "this" can remove all the
			// dead subscribers
			baseObject->addDisposeListener(this->shared_from_this());
		}
		virtual std::weak_ptr<IThreadObjectManager> getObjectManager() override
		{
			return this->objectManager;
		}
		virtual unsigned int getThreadId() override
		{
			return this->threadId;
		}
		virtual const IThreadObjectId & getInstanceId() override
		{
			return this->id;
		}
		virtual const T_Id & getId()
		{
			return dynamic_cast<const T_Id &>(this->id);
		}
		virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override
		{
			return std::vector<std::shared_ptr<IObservable>>();
		}
		virtual void runOnPeer(unsigned int threadId, std::function<void(std::shared_ptr<IThreadObject> peer)> toExecute) override
		{
			if (this->getThreadId() != threadId)
			{
				// if we can't get a lock on the object manager, it means this class is being destroyed
				auto objectManager = this->getObjectManager().lock();
				if (objectManager == nullptr)
				{
					return;
				}
				auto peer = objectManager->getPeer(threadId, this->shared_from_this());
				objectManager->getThreadManager()->getOrCreateThread(threadId)->addWork(std::make_shared<threadily::ThreadQueueItem>([peer, toExecute]() {
					toExecute(peer);
				}));
				return;
			}
			else
			{
				toExecute(this->shared_from_this());
			}
		}
	};

}
