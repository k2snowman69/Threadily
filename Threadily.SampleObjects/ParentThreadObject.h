#pragma once

#include <ThreadObject.h>
#include <Observable.h>
#include "ExampleThreadObject.h"

namespace threadily
{
	namespace test
	{
		class ParentThreadObject : public ThreadObject
		{
			// variables
		private:
			std::vector<std::shared_ptr<IObservable>> allObservables;
		public:
			std::shared_ptr<Observable<ExampleThreadObject>> exampleObject;
		public:
			ParentThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);

			virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
		};
	}
}