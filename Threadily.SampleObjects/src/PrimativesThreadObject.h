#pragma once

#include <string>
#include <ThreadObjectId.h>
#include <ThreadObject.h>
#include <Observable.h>

namespace threadily
{
	namespace test
	{
		// Class that helps testing of a ThreadObject that contains primative typed Observables
		class PrimativesThreadObject : public ThreadObject<>
		{
			// variables
		private:
			std::vector<std::shared_ptr<IObservable>> allObservables;
		public:
			std::shared_ptr<Observable<int>> intValue;
			std::shared_ptr<Observable<std::wstring>> name;
			std::shared_ptr<Observable<std::vector<int>>> intArray;
			std::shared_ptr<Observable<std::vector<std::wstring>>> stringArray;
		public:
			PrimativesThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id);

			virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() override;
		};
	}
}