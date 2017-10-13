#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include <ThreadObject.h>
#include <ThreadObjectManager.h>

#include "Business.h"

#include "ProductManager.h"

namespace threadily {
	namespace test {
		class BusinessManager : public threadily::ThreadObjectManager<Business>
		{
		private:
			std::shared_ptr<ProductManager> productManager;
		public:
			BusinessManager(std::shared_ptr<threadily::IThreadManager> threadManager, std::shared_ptr<ProductManager> productManager);
			std::shared_ptr<ProductManager> getProductManager();
		};
	}
}
