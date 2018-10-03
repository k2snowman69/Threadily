#pragma once

#include <ThreadObjectId.h>
#include <ThreadObjectManager.h>

namespace threadily
{
	namespace test
	{
		// A subclass of a ThreadObjectManager which keeps track of how many objects were created during it's lifetime
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

			virtual std::shared_ptr<T> createObject(unsigned int threadId, const ThreadObjectId & objectId) override
			{
				this->counter++;
				return ThreadObjectManager<T>::createObject(threadId, objectId);
			}

			unsigned int getCountOfObjectsCreated()
			{
				return this->counter;
			}
		};
	}
}