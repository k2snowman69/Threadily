#include "ThreadObject.h"

#include "ReadyEvent.h"
#include "ThreadQueueItem.h"

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily 
{

	ThreadObject::ThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id)
	{
		if (objectManager == nullptr)
		{
			throw std::runtime_error("ObjectManager must be defined");
		}
		this->objectManager = objectManager;
		this->id = id;
		this->threadId = threadId;

		this->peerObjectsToNotify = std::make_shared<std::set<std::shared_ptr<ThreadObject>>>();
		this->disposeListeners = std::make_shared<std::vector<std::weak_ptr<ThreadObject>>>();
		this->threadObjectToChangeSubscribers = std::make_shared<std::map<unsigned int, std::shared_ptr<std::vector<std::shared_ptr<ISubscribeHandle>>>>>();
	}

	ThreadObject::~ThreadObject()
	{
		auto objectManagerStrong = this->objectManager.lock();
		if (objectManagerStrong != nullptr)
		{
			auto threadManager = objectManagerStrong->getThreadManager();

			for (auto it = this->disposeListeners->begin(); it != this->disposeListeners->end(); ++it)
			{
				std::weak_ptr<ThreadObject> listenerWeak = *it;
				if (!listenerWeak.expired())
				{
					std::shared_ptr<ThreadObject> listener = listenerWeak.lock();
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

	unsigned int ThreadObject::getThreadId()
	{
		return this->threadId;
	}

	unsigned int ThreadObject::getId()
	{
		return this->id;
	}
	std::weak_ptr<IThreadObjectManager> ThreadObject::getObjectManager()
	{
		return this->objectManager;
	}

	std::vector<std::shared_ptr<IObservable>> ThreadObject::getObservableProperties()
	{
		return std::vector<std::shared_ptr<IObservable>>();
	}

	void ThreadObject::initLinks(std::shared_ptr<ThreadObject> baseObject)
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

	void ThreadObject::addPeerObjectToNotify(std::shared_ptr<ThreadObject> peerObject)
	{
		this->peerObjectsToNotify->insert(peerObject);
	}

	void ThreadObject::addDisposeListener(std::shared_ptr<ThreadObject> thingToNotify)
	{
		this->disposeListeners->push_back(thingToNotify);
	}

	std::shared_ptr<ThreadObject> ThreadObject::getReference(std::shared_ptr<ThreadObject> value)
	{
		return value;
	}

#ifdef EMSCRIPTEN
	EMSCRIPTEN_BINDINGS(ThreadObject) {
		class_<IThreadObject>("IThreadObject")
			.smart_ptr<std::shared_ptr<IThreadObject>>("IThreadObject")
			.function("getThreadId", &IThreadObject::getThreadId, pure_virtual())
			.function("getId", &IThreadObject::getId, pure_virtual())
			;
		class_<ThreadObject, base<threadily::IThreadObject>>("ThreadObject")
			.constructor<std::shared_ptr<IThreadObjectManager>, unsigned int, unsigned int>()
			.smart_ptr<std::shared_ptr<ThreadObject>>("ThreadObject")
			.class_function("getReference", &ThreadObject::getReference)
			;
	}
#endif
}