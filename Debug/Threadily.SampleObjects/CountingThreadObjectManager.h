#pragma once

#include <ThreadObjectManager.h>

#include "ExampleThreadObject.h"

namespace threadily
{
	namespace test
	{
		template <typename T>
		class CountingThreadObjectManager : public ThreadObjectManager<T>
		{
		private:
			unsigned int counter;
			// variables
		public:
			CountingThreadObjectManager(std::shared_ptr<IThreadManager> threadManager)
			: ThreadObjectManager<T>(threadManager)
			{
				this->counter = 0;
			}

			virtual std::shared_ptr<T> createObject(unsigned int threadId, int objectId) override
			{
				this->counter++;
				return ThreadObjectManager<T>::createObject(threadId, objectId);
			}

			unsigned int getCreatedCount()
			{
				return this->counter;
			}
		};
	}
}