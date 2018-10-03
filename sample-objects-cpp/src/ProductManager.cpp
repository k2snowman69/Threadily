#include "ProductManager.h"

#include <string>

namespace threadily {
	namespace test {
		ProductManager::ProductManager(std::shared_ptr<threadily::IThreadManager> threadManager) : threadily::ThreadObjectManager<Product, ProductId>(threadManager)
		{
		}
		std::shared_ptr<Product> ProductManager::getOrCreateObject(unsigned int threadId, unsigned long businessId, unsigned long productId)
		{
			// it doesn't so we need to create it
			auto customId = ProductId(businessId, productId);
			return threadily::ThreadObjectManager<Product, ProductId>::getOrCreateObject(threadId, customId);
		}
	}
}
