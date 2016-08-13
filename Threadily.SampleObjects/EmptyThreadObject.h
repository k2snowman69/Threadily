#pragma once

#include <string>
#include <ThreadObject.h>
#include <Observable.h>

namespace threadily
{
	namespace test
	{
		class EmptyThreadObject : public ThreadObject
		{
			// variables
		public:
			EmptyThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);

			virtual unsigned int getThreadId() override;
			virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
		};
	}
}