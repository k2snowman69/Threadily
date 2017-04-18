#include "ProductManager.h"

#include <string>

namespace threadily {
	namespace test {
		ProductManager::ProductManager(std::shared_ptr<threadily::IThreadManager> threadManager) : threadily::ThreadObjectManager<Product>(threadManager)
		{
		}
		std::shared_ptr<Product> ProductManager::getOrCreateObject(unsigned int threadId, unsigned long businessId, unsigned long productId)
		{
			// See if it already exists
			for (auto it = this->productIdToInstanceId.begin(); it != this->productIdToInstanceId.end(); ++it) {
				if (it->businessId == businessId &&
					it->productId == productId) {
					return threadily::ThreadObjectManager<Product>::getOrCreateObject(threadId, *it);
				}
			}

			// it doesn't so we need to create it
			auto customId = ProductId(this->productIdToInstanceId.size(), businessId, productId);
			this->productIdToInstanceId.push_back(customId);
			return threadily::ThreadObjectManager<Product>::getOrCreateObject(threadId, customId);
		}
	}
}
