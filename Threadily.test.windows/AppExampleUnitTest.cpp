#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"
#include "ReadyEvent.h"

#include "AppFactory.h"
#include "App.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily {
	namespace test {
		TEST_CLASS(AppExampleUnitTest)
		{
		public:

			/**
			* Basically tries to create everything in the system and any linkages in the system
			*/
			TEST_METHOD(ReadBusinessesAsync_VerifyNewBusinessIsOnUiThread)
			{
				auto app = AppFactory::getInstance().create();

				// first see if the business exists
				waitForAsync(
					app->isBusinessesPending,
					[&app]() {
					app->readBusinessesAsync(0, 10, "Wit");
				});
				Assert::AreEqual(0, (int)app->businesses->size(), L"Should be 0 businesses in the system");

				// Since it doesn't exist, we should make it!
				waitForAsync(
					app->isCreateBusinessPending,
					[&app]() {
					app->createBusinessAsync("Witness");
				});
				auto witness = app->createdBusiness->get();
				Assert::AreEqual(std::string("Witness"), witness->name->get());

				auto similarToThreadilyBridge = witness->products->subscribe([](std::shared_ptr<Product> newValue, size_t index, threadily::ObservableActionType action) {
				});

				// Now let's re-query that business again and check that it's on the right thread
				waitForAsync(
					app->isBusinessesPending,
					[&app]() {
					app->readBusinessesAsync(0, 10, "Wit");
				});
				Assert::AreEqual(1, (int)app->businesses->size(), L"Should be 1 businesses in the system");
				Assert::AreEqual((int)ThreadIds::ThreadId_UI, (int)app->businesses->at(0)->getThreadId(), L"Verify the business is on the UI thread");
			}

			/**
			* Basically tries to create everything in the system and any linkages in the system
			*/
			TEST_METHOD(ReadProductsAsync_bug_2017_03_18)
			{
				auto app = AppFactory::getInstance().create();

				// first see if the business exists
				waitForAsync(
					app->isBusinessesPending,
					[&app]() {
					app->readBusinessesAsync(0, 10, "Wit");
				});
				Assert::AreEqual(0, (int)app->businesses->size(), L"Should be 0 businesses in the system");

				// Since it doesn't exist, we should make it!
				waitForAsync(
					app->isCreateBusinessPending,
					[&app]() {
					app->createBusinessAsync("Witness");
				});
				auto witness = app->createdBusiness->get();
				Assert::AreEqual(std::string("Witness"), witness->name->get());

				auto similarToThreadilyBridge = witness->products->subscribe([](std::shared_ptr<Product> newValue, size_t index, threadily::ObservableActionType action) {
				});

				// Even though we just created the business let's check it's product listings anyway
				waitForAsync(
					witness->isProductsPending,
					[&witness]() {
					witness->readProductsAsync(0, 20, "");
				});
				Assert::AreEqual(0, (int)witness->products->size(), L"Should be 0 products for the business in the system");

				// create a product
				waitForAsync(
					witness->isCreateProductPending,
					[&witness]() {
					witness->createProductAsync("Name");
				});
				Assert::IsTrue(witness->createdProduct->get() != nullptr, L"Product should not be null after creating it");

				// Make sure the product exists
				waitForAsync(
					witness->isProductsPending,
					[&witness]() {
					witness->readProductsAsync(0, 20, "");
				});
				Assert::AreEqual(1, (int)witness->products->size(), L"Should be 1 products for the business in the system");
			}

			// https://github.com/k2snowman69/Threadily/issues/7
			TEST_METHOD(BusinessVectorIncreasesRefCount_issue_7)
			{
				auto app = AppFactory::getInstance().create();

				// Create a business
				waitForAsync(
					app->isCreateBusinessPending,
					[&app]() {
					app->createBusinessAsync("Witness");
				});
				auto witness = app->createdBusiness->get();
				Assert::AreEqual(std::string("Witness"), witness->name->get());

				auto oldUseCount = witness.use_count();

				// Now query the business populating app->businesses
				waitForAsync(
					app->isBusinessesPending,
					[&app]() {
					app->readBusinessesAsync(0, 10, "Wit");
				});
				Assert::AreEqual(1, (int)app->businesses->size(), L"Should be 1 businesses in the system");

				Assert::AreEqual(oldUseCount + 1, witness.use_count(), L"Expected the use count to increase by one as it should now be stored in the businesses listing");
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