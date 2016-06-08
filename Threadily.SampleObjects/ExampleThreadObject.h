#pragma once

#include <string>
#include <ThreadObject.h>
#include <Observable.h>

namespace threadily
{
	namespace test
	{
		class ExampleThreadObject : public ThreadObject
		{
			// variables
		private:
			std::vector<std::shared_ptr<IObservable>> allObservables;
		public:
			std::shared_ptr<Observable<std::wstring>> name;
		public:
			ExampleThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);

			virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
		};
	}
}