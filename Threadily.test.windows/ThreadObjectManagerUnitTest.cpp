#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include <ThreadManager.h>
#include <ThreadObjectManager.h>

#include "CountingThreadObjectManager.h"
#include "PrimativesThreadObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ThreadableObjectManagerUnitTest)
		{
		public:

			// Creates and destroys the same object a few times
			TEST_METHOD(ThreadableObjectManager_CreateDestroyLoop)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				auto manager = std::make_shared<ThreadObjectManager<ThreadObject>>(threadManager);

				auto objectUI1 = manager->getOrCreateObject(ThreadIds::UI, 0);
				Assert::IsNotNull(objectUI1.get());
				std::weak_ptr<ThreadObject> objectUI1Weak = objectUI1;
				objectUI1 = nullptr;

				Assert::IsTrue(objectUI1Weak.expired(), L"Expected the element to be deleted since there are no reference to it");

				auto objectUI2 = manager->getOrCreateObject(ThreadIds::UI, 0);
				Assert::IsNotNull(objectUI2.get());
				objectUI2 = nullptr;

				auto objectUI3 = manager->getOrCreateObject(ThreadIds::UI, 0);
				Assert::IsNotNull(objectUI3.get());
			}

			// Creates and destroys the same object a few times between two threads App <-> UI
			TEST_METHOD(ThreadableObjectManager_CreateDestroyLoop_LinkedObjects)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>({ ThreadIds::App }), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<ThreadObject>>(threadManager);

				auto objectUI1 = manager->getOrCreateObject(ThreadIds::UI, 0);
				Assert::IsNotNull(objectUI1.get());
				std::weak_ptr<ThreadObject> objectUI1Weak = objectUI1;
				objectUI1 = nullptr;

				// Since it's a two way object, they hold each other alive
				Assert::IsFalse(objectUI1Weak.expired(), L"Expected the element to be deleted since there are no reference to it");
			}

			// tests to make sure two thread objects get linked together
			// App <-> UI
			TEST_METHOD(ThreadableObjectManager_ThreadObjectsLinking_1)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>({ ThreadIds::App }), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);

				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);
				auto objectApp = manager->getOrCreateObject(ThreadIds::App, 0);

				Assert::AreEqual((size_t)1, objectApp->name->getSubscribersCount(), L"Verify subscribers count");
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify subscribers count");

				objectApp = nullptr;

				// This is still 1 because the UI object holds onto a reference to the App object because it notifies it
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify subscribers count");
			}

			// tests to make sure when the linking looks like this:
			// Service <-> App <-> UI 
			// and we create object 1 and 3 first... when we create object 2 everything should hook up correctly
			TEST_METHOD(ThreadableObjectManager_ThreadObjectsLinking_2)
			{
				// create all three threads
				auto threadManager = std::make_shared<ThreadManager>();
				auto uiThread = threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>({ ThreadIds::App }), nullptr);
				auto appThread = threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::Service, ThreadIds::UI }), nullptr);
				auto serviceThread = threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);

				// create the ui first then the service thread. These should have nothing to do with one another
				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);
				auto objectService = manager->getOrCreateObject(ThreadIds::Service, 0);

				Assert::AreEqual((size_t)1, objectService->name->getSubscribersCount(), L"Verify Service subscribers count");
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify UI subscribers count");

				// now create the app thread. This should be the connecting part to the ui and service threads
				auto objectApp = manager->getOrCreateObject(appThread->getThreadId(), 0);

				Assert::AreEqual((size_t)1, objectService->name->getSubscribersCount(), L"Verify Service subscribers count");
				Assert::AreEqual((size_t)2, objectApp->name->getSubscribersCount(), L"Verify App subscribers count");
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify UI subscriber count");

				objectApp = nullptr;

				Assert::AreEqual((size_t)1, objectService->name->getSubscribersCount(), L"Verify Service subscribers count");
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify UI subscriber count");
			}

			// tests to make sure one way linking works
			// App -> UI
			TEST_METHOD(ThreadableObjectManager_ThreadObjectsLinking_OneWay_UIFirst)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>(), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);

				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);
				auto objectApp = manager->getOrCreateObject(ThreadIds::App, 0);

				Assert::AreEqual((size_t)1, objectApp->name->getSubscribersCount(), L"Verify subscribers count");
				Assert::AreEqual((size_t)0, objectUI->name->getSubscribersCount(), L"Verify subscribers count");

				objectApp = nullptr;

				Assert::AreEqual((size_t)0, objectUI->name->getSubscribersCount(), L"Verify subscribers count");
			}

			// tests to make sure one way linking works
			// App -> UI
			TEST_METHOD(ThreadableObjectManager_ThreadObjectsLinking_OneWay_AppFirst)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>(), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);

				auto objectApp = manager->getOrCreateObject(ThreadIds::App, 0);
				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);

				Assert::AreEqual((size_t)1, objectApp->name->getSubscribersCount(), L"Verify subscribers count");
				Assert::AreEqual((size_t)0, objectUI->name->getSubscribersCount(), L"Verify subscribers count");

				objectApp = nullptr;

				Assert::AreEqual((size_t)0, objectUI->name->getSubscribersCount(), L"Verify subscribers count");
			}

			// tests to make sure one way linking works
			// App <- UI
			TEST_METHOD(ThreadableObjectManager_ThreadObjectsLinking_OneWayDown_UIFirst)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>({ ThreadIds::App }), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>(), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);

				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);
				auto objectApp = manager->getOrCreateObject(ThreadIds::App, 0);

				Assert::AreEqual((size_t)0, objectApp->name->getSubscribersCount(), L"Verify subscribers count");
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify subscribers count");

				objectApp = nullptr;

				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify subscribers count");
			}

			// tests to make sure one way linking works
			// App <- UI
			TEST_METHOD(ThreadableObjectManager_ThreadObjectsLinking_OneWayDown_AppFirst)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>({ ThreadIds::App }), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>(), nullptr);

				auto manager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);

				auto objectApp = manager->getOrCreateObject(ThreadIds::App, 0);
				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);

				Assert::AreEqual((size_t)0, objectApp->name->getSubscribersCount(), L"Verify subscribers count");
				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify subscribers count");

				objectApp = nullptr;

				Assert::AreEqual((size_t)1, objectUI->name->getSubscribersCount(), L"Verify subscribers count");
			}

			TEST_METHOD(ThreadableObjectManager_Subclassing_1)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::UI, std::set<unsigned int>(), nullptr);
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }), nullptr);

				auto manager = std::make_shared<CountingThreadObjectManager<PrimativesThreadObject>>(threadManager);
				auto objectUI = manager->getOrCreateObject(ThreadIds::UI, 0);
				auto objectApp = manager->getOrCreateObject(ThreadIds::App, 0);

				Assert::AreEqual(2U, manager->getCountOfObjectsCreated(), L"Expected our custom creation method to be called");
			}
		};
	}
}