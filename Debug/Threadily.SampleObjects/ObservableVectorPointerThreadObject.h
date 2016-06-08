#pragma once

#include <ThreadObject.h>
#include <Observable.h>

namespace threadily
{
	namespace test
	{
		class ObservableVectorPointerThreadObject : public ThreadObject
		{
			// variables
		private:
			std::vector<std::shared_ptr<IObservable>> allObservables;
		public:
			std::shared_ptr<Observable<std::vector<int>>> idList;
		public:
			ObservableVectorPointerThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);

			virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
		};
	}
}