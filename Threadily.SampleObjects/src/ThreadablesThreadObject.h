#pragma once

#include <string>
#include <ThreadObjectId.h>
#include <ThreadObject.h>
#include <Observable.h>

#include "EmptyThreadObject.h"

namespace threadily
{
	namespace test
	{
		// Class that helps testing of a ThreadObject that contains ThreadObject typed Observables
		class ThreadablesThreadObject : public ThreadObject<>
		{
			// variables
		private:
			std::vector<std::shared_ptr<IObservable>> allObservables;
		public:
			std::shared_ptr<Observable<EmptyThreadObject>> emptyObject;
			std::shared_ptr<Observable<std::vector<EmptyThreadObject>>> emptyObjectArray;
		public:
			ThreadablesThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id);

			virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
		};
	}
}