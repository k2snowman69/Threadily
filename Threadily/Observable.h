#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>

#include "IObservable.h"
#include "IThreadObjectManager.h"
#include "IThreadObject.h"
#include "IThreadQueueItemManager.h"

#include "ThreadQueueItem.h"

/// Template for how to create callback handlers for Observable types. Use before the EMSCRIPTEN_BINDINGS line.

#define defineCallbackHandler(typeName, externalNameExtension) \
	class ISubscribeHandle##externalNameExtension##CallbackWrapper : public wrapper<threadily::ISubscribeHandleCallback<typeName>> \
	{ \
	public: \
		EMSCRIPTEN_WRAPPER(ISubscribeHandle##externalNameExtension##CallbackWrapper); \
		void onChange(typeName work) { \
			return call<void>("onChange", work); \
		} \
	}; \

#define defineThreadObjectCallbackHandler(typeName, externalNameExtension) \
	class ISubscribeHandle##externalNameExtension##CallbackWrapper : public wrapper<threadily::ISubscribeHandleCallback<typeName>> \
	{ \
	public: \
		EMSCRIPTEN_WRAPPER(ISubscribeHandle##externalNameExtension##CallbackWrapper); \
		void onChange(std::shared_ptr<typeName> work) { \
			return call<void>("onChange", work); \
		} \
	}; \

/// Template for how to create callback handlers for Observable Vector types. Use before the EMSCRIPTEN_BINDINGS line.
#define defineCallbackVectorHandler(typeName, externalNameExtension) \
	class ISubscribeHandle##externalNameExtension##VectorCallbackWrapper : public wrapper<threadily::ISubscribeHandleVectorCallback<typeName>> \
	{ \
	public: \
		EMSCRIPTEN_WRAPPER(ISubscribeHandle##externalNameExtension##VectorCallbackWrapper); \
		void onChange(typeName work, size_t index, threadily::ObservableActionType action) { \
			return call<void>("onChange", work, index, action); \
		} \
	}; \

/// Template for how to create callback handlers for Observable Vector types. Use before the EMSCRIPTEN_BINDINGS line.
#define defineThreadObjectCallbackVectorHandler(typeName, externalNameExtension) \
	class ISubscribeHandle##externalNameExtension##VectorCallbackWrapper : public wrapper<threadily::ISubscribeHandleVectorCallback<typeName>> \
	{ \
	public: \
		EMSCRIPTEN_WRAPPER(ISubscribeHandle##externalNameExtension##VectorCallbackWrapper); \
		void onChange(std::shared_ptr<typeName> work, size_t index, threadily::ObservableActionType action) { \
			return call<void>("onChange", work, index, action); \
		} \
	}; \

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)
/// Template for how to create Observable types. Use inside the EMSCRIPTEN_BINDINGS line.
#define defineObservable(typeName, externalNameExtension) \
	class_<threadily::Observable<typeName>>(EXPAND_AND_QUOTE(Observable##externalNameExtension)) \
		.smart_ptr<std::shared_ptr<threadily::Observable<typeName>>>(EXPAND_AND_QUOTE(Observable##externalNameExtension)) \
		.function("get", &threadily::Observable<typeName>::get) \
		.function("set", &threadily::Observable<typeName>::set) \
		.function("subscribe", select_overload<std::shared_ptr<threadily::ISubscribeHandle>(threadily::ISubscribeHandleCallback<typeName>*)>(&threadily::Observable<typeName>::subscribe), allow_raw_pointers()) \
		; \
	class_<threadily::ISubscribeHandleCallback<typeName>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##Callback)) \
		.smart_ptr<std::shared_ptr<threadily::ISubscribeHandleCallback<typeName>>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##Callback)) \
		.function("onChange", &threadily::ISubscribeHandleCallback<typeName>::onChange, pure_virtual()) \
		.allow_subclass<ISubscribeHandle##externalNameExtension##CallbackWrapper>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##CallbackWrapper)) \
		; \

/// Template for how to create Observable vector types. Use inside the EMSCRIPTEN_BINDINGS line.
#define defineObservableVector(typeName, externalNameExtension) \
	class_<threadily::Observable<std::vector<typeName>>>(EXPAND_AND_QUOTE(ObservableVector##externalNameExtension)) \
		.smart_ptr<std::shared_ptr<threadily::Observable<std::vector<typeName>>>>(EXPAND_AND_QUOTE(ObservableVector##externalNameExtension)) \
		.function("size", &threadily::Observable<std::vector<typeName>>::size) \
		.function("at", &threadily::Observable<std::vector<typeName>>::at) \
		.function("insert", &threadily::Observable<std::vector<typeName>>::insert) \
		.function("set", &threadily::Observable<std::vector<typeName>>::set) \
		.function("erase", &threadily::Observable<std::vector<typeName>>::erase) \
		.function("subscribe", select_overload<std::shared_ptr<threadily::ISubscribeHandle>(threadily::ISubscribeHandleVectorCallback<typeName>*)>(&threadily::Observable<std::vector<typeName>>::subscribe), allow_raw_pointers()) \
		; \
	class_<threadily::ISubscribeHandleVectorCallback<typeName>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##VectorCallback)) \
		.smart_ptr<std::shared_ptr<threadily::ISubscribeHandleVectorCallback<typeName>>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##VectorCallback)) \
		.function("onChange", &threadily::ISubscribeHandleVectorCallback<typeName>::onChange, pure_virtual()) \
		.allow_subclass<ISubscribeHandle##externalNameExtension##VectorCallbackWrapper>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##VectorCallbackWrapper)) \
		; \

/// Template for how to create Observable types. Use inside the EMSCRIPTEN_BINDINGS line.
#define defineThreadObjectObservable(typeName, externalNameExtension) \
	class_<threadily::Observable<typeName>>(EXPAND_AND_QUOTE(Observable##externalNameExtension)) \
		.smart_ptr<std::shared_ptr<threadily::Observable<typeName>>>(EXPAND_AND_QUOTE(Observable##externalNameExtension)) \
		.function("get", &threadily::Observable<typeName>::get) \
		.function("set", &threadily::Observable<typeName>::set) \
		.function("subscribe", select_overload<std::shared_ptr<threadily::ISubscribeHandle>(threadily::ISubscribeHandleCallback<typeName>*)>(&threadily::Observable<typeName>::subscribe), allow_raw_pointers()) \
		; \
	class_<threadily::ISubscribeHandleCallback<typeName>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##Callback)) \
		.smart_ptr<std::shared_ptr<threadily::ISubscribeHandleCallback<typeName>>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##Callback)) \
		.function("onChange", &threadily::ISubscribeHandleCallback<typeName>::onChange, pure_virtual()) \
		.allow_subclass<ISubscribeHandle##externalNameExtension##CallbackWrapper>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##CallbackWrapper)) \
		; \

// std::shared_ptr<Observable<std::vector<EmptyThreadObject>>>
/// Template for how to create Observable vector types. Use inside the EMSCRIPTEN_BINDINGS line.
#define defineThreadObjectObservableVector(typeName, externalNameExtension) \
	class_<threadily::Observable<std::vector<typeName>>>(EXPAND_AND_QUOTE(ObservableVector##externalNameExtension)) \
		.smart_ptr<std::shared_ptr<threadily::Observable<std::vector<typeName>>>>(EXPAND_AND_QUOTE(ObservableVector##externalNameExtension)) \
		.function("size", &threadily::Observable<std::vector<typeName>>::size) \
		.function("at", &threadily::Observable<std::vector<typeName>>::at) \
		.function("insert", &threadily::Observable<std::vector<typeName>>::insert) \
		.function("set", &threadily::Observable<std::vector<typeName>>::set) \
		.function("erase", &threadily::Observable<std::vector<typeName>>::erase) \
		.function("subscribe", select_overload<std::shared_ptr<threadily::ISubscribeHandle>(threadily::ISubscribeHandleVectorCallback<typeName>*)>(&threadily::Observable<std::vector<typeName>>::subscribe), allow_raw_pointers()) \
		; \
	class_<threadily::ISubscribeHandleVectorCallback<typeName>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##VectorCallback)) \
		.smart_ptr<std::shared_ptr<threadily::ISubscribeHandleVectorCallback<typeName>>>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##VectorCallback)) \
		.function("onChange", &threadily::ISubscribeHandleVectorCallback<typeName>::onChange, pure_virtual()) \
		.allow_subclass<ISubscribeHandle##externalNameExtension##VectorCallbackWrapper>(EXPAND_AND_QUOTE(ISubscribeHandle##externalNameExtension##VectorCallbackWrapper)) \
		; \

namespace threadily {

	template <class T, class Enable = void>
	class ISubscribeHandleCallback
	{
	public:
		virtual void onChange(T newValue) = 0;
	};

	template <class T>
	class ISubscribeHandleCallback<T, typename std::enable_if<std::is_base_of<IThreadObject, T>::value>::type>
	{
	public:
		virtual void onChange(std::shared_ptr<T> newValue) = 0;
	};

	// base template class for observables
	template <class T, class Enable = void>
	class SubscribeHandle : public ISubscribeHandle
	{
	private:
		ISubscribeHandleCallback<T>* callback;
		std::function<void(T newValue)> handler;
	public:
		SubscribeHandle(std::function<void(T newValue)> handler)
		{
			if (handler == nullptr)
			{
				throw std::runtime_error("argument 'handler' required");
			}
			callback = nullptr;
			this->handler = handler;
		}
		SubscribeHandle(ISubscribeHandleCallback<T>* callback)
		{
			this->callback = callback;
			this->handler = nullptr;
		}
		~SubscribeHandle()
		{

		}
		void runCallback(T newValue)
		{
			if (this->callback != nullptr)
			{
				this->callback->onChange(newValue);
			}
			else
			{
				this->handler(newValue);
			}
		}
	};

	// base template class for observables
	template <class T>
	class SubscribeHandle <T, typename std::enable_if<std::is_base_of<IThreadObject, T>::value>::type> : public ISubscribeHandle
	{
	private:
		ISubscribeHandleCallback<T>* callback;
		std::function<void(std::shared_ptr<T> newValue)> handler;
	public:
		SubscribeHandle(std::function<void(std::shared_ptr<T> newValue)> handler)
		{
			if (handler == nullptr)
			{
				throw std::runtime_error("argument 'handler' required");
			}
			callback = nullptr;
			this->handler = handler;
		}
		SubscribeHandle(ISubscribeHandleCallback<T>* callback)
		{
			this->callback = callback;
			this->handler = nullptr;
		}
		~SubscribeHandle()
		{

		}
		void runCallback(std::shared_ptr<T> newValue)
		{
			if (this->callback != nullptr)
			{
				this->callback->onChange(newValue);
			}
			else
			{
				this->handler(newValue);
			}
		}
	};

	template <class T, class Enable = void>
	class Observable : public IObservable
	{
	private:
		T value;
		std::vector<std::weak_ptr<SubscribeHandle<T>>> subscribers;
	public:
		Observable()
		{
			value = T();
			this->subscribers = std::vector<std::weak_ptr<SubscribeHandle<T>>>();
		};

		T get()
		{
			return this->value;
		}

		void set(T newValue)
		{
			if (this->value != newValue)
			{
				this->value = newValue;

				// notify all handlers, if we couldn't get a lock, queue that handle for removal
				auto it = this->subscribers.begin();
				while (it != this->subscribers.end())
				{
					if (std::shared_ptr<SubscribeHandle<T>> subscriber = it->lock())
					{
						subscriber->runCallback(newValue);
						++it;
					}
					else
					{
						it = this->subscribers.erase(it);
					}
				}
			}
		}

		std::shared_ptr<ISubscribeHandle> subscribe(std::function<void(T newValue)> handler)
		{
			std::shared_ptr<SubscribeHandle<T>> newSub = std::make_shared<SubscribeHandle<T>>(handler);
			this->subscribers.push_back(newSub);

			return newSub;
		}

		std::shared_ptr<ISubscribeHandle> subscribe(ISubscribeHandleCallback<T>* handler)
		{
			std::shared_ptr<SubscribeHandle<T>> newSub = std::make_shared<SubscribeHandle<T>>(handler);
			this->subscribers.push_back(newSub);

			return newSub;
		}

		void unsubscribe(std::shared_ptr<ISubscribeHandle> handleToRemove)
		{
			for (auto it = this->subscribers.begin(); it != this->subscribers.end(); ++it)
			{
				if (it->expired())
				{
					this->subscribers.erase(it);
				}

				auto d = it->lock();
				if (d == handleToRemove)
				{
					this->subscribers.erase(it);
					break;
				}
			}
		}

		size_t getSubscribersCount()
		{
			// clear out expired functions
			auto it = this->subscribers.begin();
			while (it != this->subscribers.end())
			{
				if (std::shared_ptr<SubscribeHandle<T>> subscriber = it->lock())
				{
					++it;
				}
				else
				{
					it = this->subscribers.erase(it);
				}
			}

			// return final size
			return this->subscribers.size();
		}

		virtual std::shared_ptr<ISubscribeHandle> subscribe(std::shared_ptr<IThreadQueueItemManager> thread, std::shared_ptr<IObservable> other) override
		{
			auto thingToNotify = std::dynamic_pointer_cast<Observable<T>>(other);
			if (thingToNotify == nullptr)
			{
				throw std::runtime_error("Current variable type and argument 'other' type are not the same. Cannot create a notification link between the two variables.");
			}

			std::function<void(T newValue)> handler = [thread, thingToNotify](T newValue)
			{
				thread->addWork(std::make_shared<ThreadQueueItem>([thingToNotify, newValue]()
				{
					thingToNotify->set(newValue);
				}));
			};

			return this->subscribe(handler);
		}
	};

	template <class T>
	class Observable<T, typename std::enable_if<std::is_base_of<IThreadObject, T>::value>::type> : public IObservable
	{
	private:
		unsigned int threadId;
		std::shared_ptr<T> value;
		std::vector<std::weak_ptr<SubscribeHandle<T>>> subscribers;
	public:
		Observable(unsigned int threadId)
		{
			this->threadId = threadId;
			this->value = nullptr;
			this->subscribers = std::vector<std::weak_ptr<SubscribeHandle<T>>>();
		};

		std::shared_ptr<T> get()
		{
			return this->value;
		}

		void set(std::shared_ptr<T> newValue)
		{
			// if newValue was set on a different thread, we need to get this threads version of that object
			if (newValue != nullptr && newValue->getThreadId() != this->threadId)
			{
				auto objectManager = newValue->getObjectManager().lock();
				if (objectManager == nullptr)
				{
					// if the object manager is null, all will be destroyed soon so no need to set the value
					return;
				}
				std::shared_ptr<IThreadObject> peerObject = objectManager->getPeer(this->threadId, newValue);
				newValue = std::static_pointer_cast<T>(peerObject);
			}
			if (this->value != newValue)
			{
				this->value = newValue;

				// notify all handlers, if we couldn't get a lock, queue that handle for removal
				auto it = this->subscribers.begin();
				while (it != this->subscribers.end())
				{
					if (std::shared_ptr<SubscribeHandle<T>> subscriber = it->lock())
					{
						subscriber->runCallback(newValue);
						++it;
					}
					else
					{
						it = this->subscribers.erase(it);
					}
				}
			}
		}

		std::shared_ptr<ISubscribeHandle> subscribe(std::function<void(std::shared_ptr<T> newValue)> handler)
		{
			std::shared_ptr<SubscribeHandle<T>> newSub = std::make_shared<SubscribeHandle<T>>(handler);
			this->subscribers.push_back(newSub);

			return newSub;
		}

		std::shared_ptr<ISubscribeHandle> subscribe(ISubscribeHandleCallback<T>* handler)
		{
			std::shared_ptr<SubscribeHandle<T>> newSub = std::make_shared<SubscribeHandle<T>>(handler);
			this->subscribers.push_back(newSub);

			return newSub;
		}

		void unsubscribe(std::shared_ptr<ISubscribeHandle> handleToRemove)
		{
			for (auto it = this->subscribers.begin(); it != this->subscribers.end(); ++it)
			{
				if (it->expired())
				{
					this->subscribers.erase(it);
				}

				auto d = it->lock();
				if (d == handleToRemove)
				{
					this->subscribers.erase(it);
					break;
				}
			}
		}

		size_t getSubscribersCount()
		{
			// clear out expired functions
			auto it = this->subscribers.begin();
			while (it != this->subscribers.end())
			{
				if (std::shared_ptr<SubscribeHandle<T>> subscriber = it->lock())
				{
					++it;
				}
				else
				{
					it = this->subscribers.erase(it);
				}
			}

			// return final size
			return this->subscribers.size();
		}

		virtual std::shared_ptr<ISubscribeHandle> subscribe(std::shared_ptr<IThreadQueueItemManager> thread, std::shared_ptr<IObservable> other) override
		{
			auto thingToNotify = std::static_pointer_cast<Observable<T>>(other);
			if (thingToNotify == nullptr)
			{
				throw std::runtime_error("Current variable type and argument 'other' type are not the same. Cannot create a notification link between the two variables.");
			}

			std::function<void(std::shared_ptr<T> newValue)> handler = [thread, thingToNotify](std::shared_ptr<T> newValue)
			{
				thread->addWork(std::make_shared<ThreadQueueItem>([thingToNotify, newValue]()
				{
					thingToNotify->set(newValue);
				}));
			};

			return this->subscribe(handler);
		}
	};

	// Vector override
	enum ObservableActionType
	{
		Insert = 0,
		Erase = 1,
		Set = 2,
	};

	template<class T, class Enable = void>
	class ISubscribeHandleVectorCallback
	{
	public:
		virtual void onChange(T newValue, size_t index, ObservableActionType action) = 0;
	};

	template <class T>
	class ISubscribeHandleVectorCallback <T, typename std::enable_if<std::is_base_of<IThreadObject, T>::value>::type>
	{
	public:
		virtual void onChange(std::shared_ptr<T> newValue, size_t index, ObservableActionType action) = 0;
	};

	template<class T>
	class SubscribeHandle<std::vector<T>, typename std::enable_if<!std::is_base_of<IThreadObject, T>::value>::type> : public ISubscribeHandle
	{
	private:
		ISubscribeHandleVectorCallback<T>* callback;
		std::function<void(T newValue, size_t index, ObservableActionType action)> handler;
	public:
		SubscribeHandle(std::function<void(T newValue, size_t index, ObservableActionType action)> handler)
		{
			if (handler == nullptr)
			{
				throw std::runtime_error("argument 'handler' required");
			}
			callback = nullptr;
			this->handler = handler;
		}
		SubscribeHandle(ISubscribeHandleVectorCallback<T>* callback)
		{
			this->callback = callback;
			this->handler = nullptr;
		}
		~SubscribeHandle()
		{

		}
		void runCallback(T newValue, size_t index, ObservableActionType action)
		{
			if (this->callback != nullptr)
			{
				this->callback->onChange(newValue, index, action);
			}
			else
			{
				this->handler(newValue, index, action);
			}
		}
	};

	template<class T>
	class SubscribeHandle<std::vector<T>, typename std::enable_if<std::is_base_of<IThreadObject, T>::value>::type> : public ISubscribeHandle
	{
	private:
		ISubscribeHandleVectorCallback<T>* callback;
		std::function<void(std::shared_ptr<T> newValue, size_t index, ObservableActionType action)> handler;
	public:
		SubscribeHandle(std::function<void(std::shared_ptr<T> newValue, size_t index, ObservableActionType action)> handler)
		{
			if (handler == nullptr)
			{
				throw std::runtime_error("argument 'handler' required");
			}
			callback = nullptr;
			this->handler = handler;
		}
		SubscribeHandle(ISubscribeHandleVectorCallback<T>* callback)
		{
			this->callback = callback;
			this->handler = nullptr;
		}
		~SubscribeHandle()
		{

		}
		void runCallback(std::shared_ptr<T> newValue, size_t index, ObservableActionType action)
		{
			if (this->callback != nullptr)
			{
				this->callback->onChange(newValue, index, action);
			}
			else
			{
				this->handler(newValue, index, action);
			}
		}
	};

	template<class T>
	class Observable<std::vector<T>, typename std::enable_if<!std::is_base_of<IThreadObject, T>::value>::type> : public IObservable
	{
	private:
		std::vector<T> value;
		std::vector<std::weak_ptr<SubscribeHandle<std::vector<T>>>> listOpSubscribers;

		void notifySubscribers(T value, size_t index, ObservableActionType action)
		{
			// notify all handlers, if we couldn't get a lock, queue that handle for removal
			auto it = this->listOpSubscribers.begin();
			while (it != this->listOpSubscribers.end())
			{
				if (std::shared_ptr<SubscribeHandle<std::vector<T>>> subscriber = it->lock())
				{
					subscriber->runCallback(value, index, action);
					++it;
				}
				else
				{
					it = this->listOpSubscribers.erase(it);
				}
			}
		}
	public:
		Observable()
		{
			this->value = std::vector<T>();
			this->listOpSubscribers = std::vector<std::weak_ptr<SubscribeHandle<std::vector<T>>>>();
		}

		size_t size()
		{
			return this->value.size();
		}

		T at(size_t index)
		{
			return this->value.at(index);
		}

		void insert(size_t index, T value)
		{
			while (this->value.size() < index)
			{
				this->value.emplace_back();
			}
			auto it = std::next(this->value.begin(), index);

			this->value.insert(it, value);

			this->notifySubscribers(value, index, ObservableActionType::Insert);
		}

		void set(size_t index, T value)
		{
			// if its actually an insert
			if (this->value.size() <= index)
			{
				this->insert(index, value);
			}
			// if it is a replace
			else
			{
				this->value[index] = value;

				this->notifySubscribers(value, index, ObservableActionType::Set);
			}
		}

		void erase(size_t index)
		{
			auto result = this->at(index);

			auto it = std::next(this->value.begin(), index);
			this->value.erase(it);

			this->notifySubscribers(result, index, ObservableActionType::Erase);
		}

		std::shared_ptr<ISubscribeHandle> subscribe(std::function<void(T newValue, size_t index, ObservableActionType action)> handler)
		{
			std::shared_ptr<SubscribeHandle<std::vector<T>>> newSub = std::make_shared<SubscribeHandle<std::vector<T>>>(handler);
			this->listOpSubscribers.push_back(newSub);

			return newSub;
		}

		std::shared_ptr<ISubscribeHandle> subscribe(ISubscribeHandleVectorCallback<T>* handler)
		{
			std::shared_ptr<SubscribeHandle<std::vector<T>>> newSub = std::make_shared<SubscribeHandle<std::vector<T>>>(handler);
			this->listOpSubscribers.push_back(newSub);

			return newSub;
		}

		void unsubscribe(std::shared_ptr<ISubscribeHandle> handleToRemove)
		{
			for (auto it = this->listOpSubscribers.begin(); it != this->listOpSubscribers.end(); ++it)
			{
				if (it->expired())
				{
					this->listOpSubscribers.erase(it);
				}

				auto d = it->lock();
				if (d == handleToRemove)
				{
					this->listOpSubscribers.erase(it);
					break;
				}
			}
		}
		virtual std::shared_ptr<ISubscribeHandle> subscribe(std::shared_ptr<IThreadQueueItemManager> thread, std::shared_ptr<IObservable> other) override
		{
			auto thingToNotify = std::dynamic_pointer_cast<Observable<std::vector<T>>>(other);
			if (thingToNotify == nullptr)
			{
				throw std::runtime_error("Current variable type and argument 'other' type are not the same. Cannot create a notification link between the two variables.");
			}

			std::function<void(T newValue, size_t index, ObservableActionType action)> handler = [thread, thingToNotify](T newValue, size_t index, ObservableActionType action)
			{
				thread->addWork(std::make_shared<ThreadQueueItem>([thingToNotify, newValue, index, action]()
				{
					switch (action)
					{
					case ObservableActionType::Insert:
						thingToNotify->insert(index, newValue);
						break;
					case ObservableActionType::Erase:
						thingToNotify->erase(index);
						break;
					case ObservableActionType::Set:
						thingToNotify->set(index, newValue);
						break;
					default:
						throw std::runtime_error("Observable<vector<T>::subscribe - ObservableActionType not supported");
					}
				}));
			};

			return this->subscribe(handler);
		}
	};

	// ObservableVector for IThreadObjects
	template<class T>
	class Observable<std::vector<T>, typename std::enable_if<std::is_base_of<IThreadObject, T>::value>::type> : public IObservable
	{
	private:
		unsigned int threadId;
		std::vector<std::shared_ptr<T>> value;
		std::vector<std::weak_ptr<SubscribeHandle<std::vector<T>>>> listOpSubscribers;

		void notifySubscribers(std::shared_ptr<T> value, size_t index, ObservableActionType action)
		{
			// notify all handlers, if we couldn't get a lock, queue that handle for removal
			auto it = this->listOpSubscribers.begin();
			while (it != this->listOpSubscribers.end())
			{
				if (std::shared_ptr<SubscribeHandle<std::vector<T>>> subscriber = it->lock())
				{
					subscriber->runCallback(value, index, action);
					++it;
				}
				else
				{
					it = this->listOpSubscribers.erase(it);
				}
			}
		}
	public:
		Observable(unsigned int threadId)
		{
			this->threadId = threadId;
			this->value = std::vector<std::shared_ptr<T>>();
			this->listOpSubscribers = std::vector<std::weak_ptr<SubscribeHandle<std::vector<T>>>>();
		};

		size_t size()
		{
			return this->value.size();
		}

		std::shared_ptr<T> at(size_t index)
		{
			return this->value.at(index);
		}

		void insert(size_t index, std::shared_ptr<T> value)
		{
			// if value was set on a different thread, we need to get this threads version of that object
			if (value != nullptr && value->getThreadId() != this->threadId)
			{
				auto objectManager = value->getObjectManager().lock();
				if (objectManager == nullptr)
				{
					// if the object manager is null, all will be destroyed soon so no need to set the value
					return;
				}
				std::shared_ptr<IThreadObject> peerObject = objectManager->getPeer(this->threadId, value);
				value = std::static_pointer_cast<T>(peerObject);
			}

			while (this->value.size() < index)
			{
				this->value.emplace_back();
			}
			auto it = std::next(this->value.begin(), index);

			this->value.insert(it, value);

			this->notifySubscribers(value, index, ObservableActionType::Insert);
		}

		void set(size_t index, std::shared_ptr<T> value)
		{
			// if value was set on a different thread, we need to get this threads version of that object
			if (value != nullptr && value->getThreadId() != this->threadId)
			{
				auto objectManager = value->getObjectManager().lock();
				if (objectManager == nullptr)
				{
					// if the object manager is null, all will be destroyed soon so no need to set the value
					return;
				}
				std::shared_ptr<IThreadObject> peerObject = objectManager->getPeer(this->threadId, value);
				value = std::static_pointer_cast<T>(peerObject);
			}

			// if its actually an insert
			if (this->value.size() <= index)
			{
				this->insert(index, value);
			}
			// if it is a replace
			else
			{
				this->value[index] = value;

				this->notifySubscribers(value, index, ObservableActionType::Set);
			}
		}

		void erase(size_t index)
		{
			auto result = this->at(index);

			auto it = std::next(this->value.begin(), index);
			this->value.erase(it);

			this->notifySubscribers(result, index, ObservableActionType::Erase);
		}

		std::shared_ptr<ISubscribeHandle> subscribe(std::function<void(std::shared_ptr<T> newValue, size_t index, ObservableActionType action)> handler)
		{
			auto newSub = std::make_shared<SubscribeHandle<std::vector<T>>>(handler);
			this->listOpSubscribers.push_back(newSub);

			return newSub;
		}
		
		std::shared_ptr<ISubscribeHandle> subscribe(ISubscribeHandleVectorCallback<T>* handler)
		{
			auto newSub = std::make_shared<SubscribeHandle<std::vector<T>>>(handler);
			this->listOpSubscribers.push_back(newSub);

			return newSub;
		}


		void unsubscribe(std::shared_ptr<ISubscribeHandle> handleToRemove)
		{
			for (auto it = this->listOpSubscribers.begin(); it != this->listOpSubscribers.end(); ++it)
			{
				if (it->expired())
				{
					this->listOpSubscribers.erase(it);
				}

				auto d = it->lock();
				if (d == handleToRemove)
				{
					this->listOpSubscribers.erase(it);
					break;
				}
			}
		}

		virtual std::shared_ptr<ISubscribeHandle> subscribe(std::shared_ptr<IThreadQueueItemManager> thread, std::shared_ptr<IObservable> other) override
		{
			auto thingToNotify = std::dynamic_pointer_cast<Observable<std::vector<T>>>(other);
			if (thingToNotify == nullptr)
			{
				throw std::runtime_error("Current variable type and argument 'other' type are not the same. Cannot create a notification link between the two variables.");
			}

			std::function<void(std::shared_ptr<T> newValue, size_t index, ObservableActionType action)> handler = [thread, thingToNotify](std::shared_ptr<T> newValue, size_t index, ObservableActionType action)
			{
				thread->addWork(std::make_shared<ThreadQueueItem>([thingToNotify, newValue, index, action]()
				{
					switch (action)
					{
					case ObservableActionType::Insert:
						thingToNotify->insert(index, newValue);
						break;
					case ObservableActionType::Erase:
						thingToNotify->erase(index);
						break;
					case ObservableActionType::Set:
						thingToNotify->set(index, newValue);
						break;
					default:
						throw std::runtime_error("Observable<vector<std::shared_ptr<T>>::subscribe - ObservableActionType not supported");
					}
				}));
			};

			return this->subscribe(handler);
		}
	};
}