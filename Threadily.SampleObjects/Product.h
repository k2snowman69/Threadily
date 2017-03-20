#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include <ThreadObject.h>

namespace threadily {
	namespace test {
		class Product : public threadily::ThreadObject
		{
		public:
			std::shared_ptr<threadily::Observable<std::string>> name;
		public:
			Product(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);
			~Product();
			virtual std::vector<std::shared_ptr<threadily::IObservable>> getObservableProperties() override;
		};
	}
}