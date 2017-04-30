#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include <ThreadObject.h>
#include <ThreadObjectManager.h>

#include "Product.h"

namespace threadily {
	namespace test {

		class ProductManager : public threadily::ThreadObjectManager<Product, ProductId>
		{
		public:
			ProductManager(std::shared_ptr<threadily::IThreadManager> threadManager);
			std::shared_ptr<Product> getOrCreateObject(unsigned int threadId, unsigned long businessId, unsigned long productId);
		};
	}
}