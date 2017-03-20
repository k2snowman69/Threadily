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

				// Since it doesn't exist, we should make it! First the address
				waitForAsync(
					app->isCreateBusinessPending,
					[&app]() {
					app->createBusinessAsync("Witness");
				});
				auto witness = app->createdBusiness->get();
				Assert::AreEqual(std::string("Witness"), witness->name->get());

				auto similarToThreadilyBridge = witness->products->subscribe([](std::shared_ptr<Product> newValue, size_t index, threadily::ObservableActionType action) {
				});

				// Even though we have the business let's check it's product listings anyway
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