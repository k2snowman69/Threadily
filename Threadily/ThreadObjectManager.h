#pragma once

#include <map>
#include <vector>
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
#include <mutex>
#endif

#include "IThreadObjectManager.h"
#include "ThreadObjectId.h"

namespace threadily {

	template <typename T, typename T_Id = ThreadObjectId>
	class ThreadObjectManager : public IThreadObjectManager, public std::enable_shared_from_this<ThreadObjectManager<T, T_Id>>
	{
	private:
		std::shared_ptr<std::map<unsigned int, std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>>>> threadIdToObjectMap; // maps threadId -> objectId -> object
		std::shared_ptr<IThreadManager> threadManager;
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
		std::mutex instances_m;
#endif
	private:
		void removeObject(unsigned int threadId, const T_Id & id)
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// this thread requires that you already have a lock on the instances mutex
			if (instances_m.try_lock())
			{
				throw std::runtime_error("Must have lock before executing function");
				return;
			}
#endif

			// create the map of objects per thread if needed
			std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>> idToObject;
			auto threadObjects_it = threadIdToObjectMap->find(threadId);
			if (threadObjects_it == threadIdToObjectMap->end())
			{
				idToObject = std::make_shared<std::map<T_Id, std::weak_ptr<T>>>();
				threadIdToObjectMap->insert(std::pair<unsigned int, std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>>>(threadId, idToObject));
			}
			else
			{
				idToObject = threadObjects_it->second;
			}

			// delete that specific item
			idToObject->erase(id);
		}

		void addObject(unsigned int threadId, const T_Id & id, std::shared_ptr<T> object)
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// this thread requires that you already have a lock on the instances mutex
			if (instances_m.try_lock())
			{
				throw std::runtime_error("Must have lock before executing function");
				return;
			}
#endif

			// create the map of objects per thread if needed
			std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>> idToObject;
			auto threadObjects_it = threadIdToObjectMap->find(threadId);
			if (threadObjects_it == threadIdToObjectMap->end())
			{
				idToObject = std::make_shared<std::map<T_Id, std::weak_ptr<T>>>();
				threadIdToObjectMap->insert(std::pair<unsigned int, std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>>>(threadId, idToObject));
			}
			else
			{
				idToObject = threadObjects_it->second;
			}

			// add the current item into that list
			idToObject->insert(std::pair<const T_Id &, std::weak_ptr<T>>(id, object));
		}

		std::shared_ptr<std::vector<std::shared_ptr<T>>> getActivePeerObjects(const T_Id & id)
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// this thread requires that you already have a lock on the instances mutex
			if (instances_m.try_lock())
			{
				throw std::runtime_error("Must have lock before executing function");
				return nullptr;
			}
#endif

			auto peers = std::make_shared<std::vector<std::shared_ptr<T>>>();
			for (auto it = threadIdToObjectMap->begin(); it != threadIdToObjectMap->end(); ++it)
			{
				auto peer = this->getObject(it->first, id);
				if (peer != nullptr)
				{
					peers->push_back(peer);
				}
			}
			return peers;
		}

		std::shared_ptr<T> getPeer(unsigned int threadId, std::shared_ptr<T> object)
		{
			T_Id customId = object->getId();

#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			std::lock_guard<std::mutex> lock(instances_m);
#endif
			return this->getOrOptionallyCreateObject(threadId, customId, true);
		}

		virtual std::shared_ptr<IThreadObject> getPeer(unsigned int threadId, std::shared_ptr<IThreadObject> object) override
		{
			auto typedObject = std::static_pointer_cast<T>(object);
			return this->getPeer(threadId, typedObject);
		}

		std::shared_ptr<T> getObject(unsigned int threadId, const T_Id & id)
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// this thread requires that you already have a lock on the instances mutex
			if (instances_m.try_lock())
			{
				throw std::runtime_error("Must have lock before executing function");
				return nullptr;
			}
#endif
			return this->getOrOptionallyCreateObject(threadId, id, false);
		}

		std::shared_ptr<T> getOrOptionallyCreateObject(unsigned int threadId, const T_Id & id, bool doCreate)
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			// this thread requires that you already have a lock on the instances mutex
			if (instances_m.try_lock())
			{
				throw std::runtime_error("Must have lock before executing function");
				return nullptr;
			}
#endif

			// get or create the list of objects for a specific thread
			auto threadObjects_it = this->threadIdToObjectMap->find(threadId);
			std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>> idToObject;
			if (threadObjects_it == threadIdToObjectMap->end())
			{
				idToObject = std::make_shared<std::map<T_Id, std::weak_ptr<T>>>();
				threadIdToObjectMap->insert(std::pair<unsigned int, std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>>>(threadId, idToObject));
			}
			else
			{
				idToObject = threadObjects_it->second;
			}

			// now that we have the list of ids to objects, get or create and add the current item into that list
			auto object_it = idToObject->find(id);
			if (object_it != idToObject->end())
			{
				// if we found an object, check to see if it's expired, if it hasn't then return that object
				std::weak_ptr<T> obj = object_it->second;
				auto sharedPtr = obj.lock();
				if (sharedPtr != nullptr)
				{
					return sharedPtr;
				}
				else
				{
					// get rid of the expired object
					idToObject->erase(id);
				}
			}

			// if we got here, we need to make the object
			if (doCreate)
			{
				auto newObject = this->createObject(threadId, id);
				this->addObject(threadId, id, newObject);

				// now if this just created object is supposed to notify
				// another object, it needs to hold onto a reference to that object
				// or else notifications may get lost when passing through multiple layers
				auto threadsToLink = this->threadManager->getOrCreateThread(threadId)->getThreadIdsThisNotifies();
				for (auto it = threadsToLink.begin(); it != threadsToLink.end(); ++it)
				{
					unsigned int threadNotified = *it;
					auto d = this->getOrOptionallyCreateObject(threadNotified, id, true);
					newObject->addPeerObjectToNotify(d);
				}

				return newObject;
			}
			else
			{
				return nullptr;
			}
		}

	public:
		ThreadObjectManager(std::shared_ptr<IThreadManager> threadManager)
		{
			static_assert(std::is_base_of<IThreadObject, T>::value, "Typename T in ThreadObjectManager<T> must be derived from IThreadObject");

			if (threadManager == nullptr)
			{
				throw std::runtime_error("ThreadManager must be defined");
			}
			this->threadManager = threadManager;

			this->threadIdToObjectMap = std::make_shared<std::map<unsigned int, std::shared_ptr<std::map<T_Id, std::weak_ptr<T>>>>>();
		}

		~ThreadObjectManager()
		{
			this->threadIdToObjectMap->clear();
		}

		virtual std::shared_ptr<T> createObject(unsigned int threadId, const T_Id & objectId)
		{
			T_Id copiedId = objectId;
			const T_Id & copiedIdRef = copiedId;
			auto me = this->shared_from_this();
			auto newObj = std::make_shared<T>(me, threadId, copiedIdRef);

			// get all the peers and make all the links between everyone
			auto peers = this->getActivePeerObjects(objectId);
			for (auto it = peers->begin(); it != peers->end(); ++it)
			{
				std::shared_ptr<T> peer = (*it);
				if (peer->getThreadId() != newObj->getThreadId())
				{
					newObj->initLinks(peer);
					peer->initLinks(newObj);
				}
			}

			return newObj;
		}

		virtual std::shared_ptr<IThreadManager> getThreadManager() override
		{
			return this->threadManager;
		}

		std::shared_ptr<T> getOrCreateObject(unsigned int threadId, const T_Id & id)
		{
#if !defined(EMSCRIPTEN) || defined(USE_PTHREADS)
			std::lock_guard<std::mutex> lock(instances_m);
#endif
			return this->getOrOptionallyCreateObject(threadId, id, true);
		}
	};

}
