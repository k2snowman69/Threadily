#include "ProductManager.h"

#include <string>

namespace threadily {
	namespace test {
		ProductManager::ProductManager(std::shared_ptr<threadily::IThreadManager> threadManager) : threadily::ThreadObjectManager<Product>(threadManager)
		{
		}
		std::shared_ptr<Product> ProductManager::getOrCreateObject(unsigned int threadId, long businessId, long productId)
		{
			return threadily::ThreadObjectManager<Product>::getOrCreateObject(threadId, productId);
		}
	}
}
