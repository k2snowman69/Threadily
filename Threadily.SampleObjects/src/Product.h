#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include "ProductId.h"
#include <ThreadObject.h>

namespace threadily {
	namespace test {
		class Product : public threadily::ThreadObject<ProductId>
		{
		public:
			std::shared_ptr<threadily::Observable<std::string>> name;
		public:
			Product(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, const ProductId & id);
			~Product();
			virtual std::vector<std::shared_ptr<threadily::IObservable>> getObservableProperties() override;
		};
	}
}