#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"
#include "ReadyEvent.h"

#include "ThreadManager.h"
#include "ProductManager.h"
#include "ProductId.h"
#include "Product.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily {
	namespace test {
		TEST_CLASS(CustomIdUnitTest)
		{
		public:


			/**
			* Basically tries to create everything in the system and any linkages in the system
			*/
			TEST_METHOD(GetOrCreateReturnsCustomId)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				auto manager = std::make_shared<ProductManager>(threadManager);

				// Create an ID object to keep referencing
				auto id = ProductId(0, 10, 11);
				Assert::AreEqual((unsigned int)10, id.businessId, L"Business Id was not set properly");
				Assert::AreEqual((unsigned int)11, id.productId, L"Product Id was not set properly");

				// create the product with the id requested
				auto objectUI = manager->getOrCreateObject(
					ThreadIds::ThreadId_UI, 
					id.businessId, 
					id.productId);
				auto product_UI = std::static_pointer_cast<Product>(objectUI);

				// verify that the ID was set properly
				Assert::AreEqual(id.businessId, product_UI->getServiceId().businessId, L"UI Business Id not correct");
				Assert::AreEqual(id.productId, product_UI->getServiceId().productId, L"UI Product Id not correct");

				// now lets get a sibling object
				ReadyEvent re;
				product_UI->runOnPeer(ThreadIds::ThreadId_Service, [&re, &id](std::shared_ptr<IThreadObject> peer) {
					auto product_Service = std::static_pointer_cast<Product>(peer);

					// verify that the ID was set properly
					Assert::AreEqual(id.businessId, product_Service->getServiceId().businessId, L"Service Business Id not correct");
					Assert::AreEqual(id.productId, product_Service->getServiceId().productId, L"Service Product Id not correct");

					// Now see if editing the old id will edit this guys Id
					id.businessId += 20;
					Assert::AreNotEqual(id.businessId, product_Service->getServiceId().businessId, L"Service Business Id not correct");
					Assert::AreEqual(id.productId, product_Service->getServiceId().productId, L"Service Product Id not correct");

					re.finished();
				});
				re.wait();
			}

			void waitForAsync(std::shared_ptr<threadily::Observable<bool>> isPending, std::function<void()> asyncMethod)
			{
				threadily::ReadyEvent ready;
				auto subscriptionHandle = isPending->subscribe([&ready](bool isPending) {
					if (!isPending)
					{
						ready.finished();
					}
				});
				asyncMethod();
				ready.wait();
				isPending->unsubscribe(subscriptionHandle);
			}
		};
	}
}