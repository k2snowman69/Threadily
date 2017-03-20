#include "AppManager.h"

namespace threadily {
	namespace test {
		AppManager::AppManager(std::shared_ptr<threadily::IThreadManager> threadManager) : threadily::ThreadObjectManager<App>(threadManager)
		{
			this->productManager = std::make_shared<ProductManager>(threadManager);
			this->businessManager = std::make_shared<BusinessManager>(threadManager, this->productManager);
		}
		std::shared_ptr<BusinessManager> AppManager::getBusinessManager()
		{
			return this->businessManager;
		}
		std::shared_ptr<ProductManager> AppManager::getProductManager()
		{
			return this->productManager;
		}
	}
}