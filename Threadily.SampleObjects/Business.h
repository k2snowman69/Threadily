#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include <ThreadObjectId.h>
#include <ThreadObject.h>
#include <ThreadObjectManager.h>
#include <stack>

#include "ThreadIds.h"

#include "Product.h"

namespace threadily {
	namespace test {
		class Business : public threadily::ThreadObject<>
		{
		public:
			std::shared_ptr<threadily::Observable<std::string>> name;
			// Products
			std::shared_ptr<threadily::Observable<bool>> isProductsPending;
			std::shared_ptr<threadily::Observable<std::vector<Product>>> products;
			std::shared_ptr<threadily::Observable<bool>> isCreateProductPending;
			std::shared_ptr<threadily::Observable<Product>> createdProduct;
		private:
			unsigned int productCounter;
		public:
			Business(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id);
			~Business();
			virtual std::vector<std::shared_ptr<threadily::IObservable>> getObservableProperties() override;
			// Products
			void createProductAsync(std::string name);
			void readProductsAsync(unsigned int index, unsigned int count, std::string name);
		};
	}
}
