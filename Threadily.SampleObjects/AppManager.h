#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include <ThreadObject.h>
#include <IThreadObjectManager.h>
#include <ThreadObjectManager.h>

#include "App.h"
#include "BusinessManager.h"
#include "ProductManager.h"

namespace threadily {
	namespace test {
		class AppManager : public threadily::ThreadObjectManager<App>
		{
		private:
			std::shared_ptr<BusinessManager> businessManager;
			std::shared_ptr<ProductManager> productManager;
		public:
			AppManager(std::shared_ptr<threadily::IThreadManager> threadManager);

		public:
			std::shared_ptr<BusinessManager> getBusinessManager();
			std::shared_ptr<ProductManager> getProductManager();
		};
	}
}