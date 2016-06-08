#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>

#include "IObservable.h"
#include "IThreadQueueItemManager.h"

#include "ThreadQueueItem.h"

namespace threadily {

	template<class T>
	class ISubscribeHandleCallback
	{
	public:
		virtual void onChange(T newValue) = 0;
	};

	// base template class for observables
	template<class T>
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

	template<class T>
	class Observable : public IObservable
	{
	private:
		T value;
		std::vector<std::weak_ptr<SubscribeHandle<T>>> subscribers;
	public:
		Observable() {};

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
				if (it == handleToRemove)
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

	// Vector override
	enum ObservableActionType
	{
		Insert = 0,
		Erase = 1,
		Set = 2,
	};

	template<class T>
	class ISubscribeHandleVectorCallback
	{
	public:
		virtual void onChange(T newValue, size_t index, ObservableActionType action) = 0;
	};

	template<class T>
	class SubscribeHandle<std::vector<T>> : public ISubscribeHandle
	{
	private:
		std::shared_ptr<ISubscribeHandleVectorCallback<T>> callback;
		std::function<void(T newValue, size_t index, ObservableActionType action)> handler;
	public:
		SubscribeHandle(std::function<void(T newValue, size_t index, ObservableActionType action)> handler)
		{
			if (handler == nullptr)
			{
				throw std::runtime_error("argument 'handler' required");
			}
			this->handler = handler;
		}
		SubscribeHandle(std::shared_ptr<ISubscribeHandleVectorCallback<T>> callback)
		{
			this->callback = callback;
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
	class Observable<std::vector<T>> : public IObservable
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

		std::shared_ptr<ISubscribeHandle> subscribe(std::shared_ptr<ISubscribeHandleVectorCallback<T>> handler)
		{
			std::shared_ptr<SubscribeHandle<std::vector<T>>> newSub = std::make_shared<SubscribeHandle<std::vector<T>>>(handler);
			this->subscribers.push_back(newSub);

			return newSub;
		}

		void unsubscribe(std::shared_ptr<ISubscribeHandle> handleToRemove)
		{
			for (auto it = this->listOpSubscribers.begin(); it != this->listOpSubscribers.end(); ++it)
			{
				if (it == handleToRemove)
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
}