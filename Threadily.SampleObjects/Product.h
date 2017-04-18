#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include "ProductId.h"
#include <ThreadObject.h>

namespace threadily {
	namespace test {
		class Product : public threadily::ThreadObject
		{
		public:
			std::shared_ptr<threadily::Observable<std::string>> name;
		public:
			Product(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id);
			~Product();
			ProductId getServiceId();
			virtual const ThreadObjectId & getId() override;
			virtual std::vector<std::shared_ptr<threadily::IObservable>> getObservableProperties() override;
		private:
			ProductId customId;
		};
	}
}