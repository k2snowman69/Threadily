#include "BusinessManager.h"

namespace threadily {
	namespace test {
		BusinessManager::BusinessManager(std::shared_ptr<threadily::IThreadManager> threadManager, std::shared_ptr<ProductManager> productManager) : threadily::ThreadObjectManager<Business>(threadManager)
		{
			this->productManager = productManager;
		}
		std::shared_ptr<ProductManager> BusinessManager::getProductManager()
		{
			return this->productManager;
		}
	}
}
