#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include "ThreadManager.h"
#include "ThreadObjectManager.h"
#include "ThreadObject.h"
#include "ExampleThreadObject.h"
#include "ObservableVectorPointerThreadObject.h"
#include "ReadyEvent.h"
#include "ParentThreadObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ThreadObjectUnitTest)
		{
		public:

			TEST_METHOD(ThreadObject_Create_NullManager)
			{
				try
				{
					auto threadObject_UI = std::make_shared<ThreadObject>(nullptr, ThreadIds::UI, 0);
					Assert::Fail(L"Thread objects need a single manager to keep them all in sync");
				}
				catch (std::runtime_error e)
				{

				}
			}
			TEST_METHOD(ThreadObject_GetPeer_Success)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }));
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }));
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);
			}
			// sets up the threads like so:
			// Service -> App -> UI
			// then posts a property change to the service thread
			// we expect this to go all the way to the UI thread even though the App object doesn't exist
			// If this test spins forever, then our test failed
			TEST_METHOD(ThreadObject_NotifiesThroughMiddleLayer)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }));
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }));
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ExampleThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				ReadyEvent e;

				auto subscribeHandle = threadObject_UI->name->subscribe([&e](std::wstring newValue)
				{
					Assert::AreEqual(std::wstring(L"hi!"), newValue, L"Expected the same string we set on the service");
					e.finished();
				});

				Assert::IsTrue(1 == threadObject_UI->name->getSubscribersCount(), L"There should be atleast 1 subscriber, the one from the test");
				Assert::IsTrue(1 == threadObject_Service->name->getSubscribersCount(), L"There should be atleast 1 subscriber, the App thread object");

				threadObject_Service->name->set(L"hi!");
				e.wait();
			}
			// sets up the threads like so:
			// Service -> App -> UI
			// then posts a property change to the service thread
			// we expect this to go all the way to the UI thread even though the App object doesn't exist
			// If this test spins forever, then our test failed
			TEST_METHOD(ThreadObject_Observable_Vector_Primatives)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }));
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }));
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ObservableVectorPointerThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				// set up a waiter until we get a notification on the UI thread that something has been completed
				ReadyEvent e;
				auto subscribeHandle = threadObject_UI->idList->subscribe([&e](int newValue, size_t index, ObservableActionType action)
				{
					e.finished();
				});

				threadObject_Service->idList->set(0, 17);
				e.wait();

				Assert::AreEqual((size_t)1, threadObject_Service->idList->size(), L"We directly added an element to service idList");
				Assert::AreEqual((size_t)1, threadObject_UI->idList->size(), L"The service should have synced to the front end by now");

				Assert::AreEqual(17, threadObject_Service->idList->at(0), L"We directly added an element to service idList");
				Assert::AreEqual(17, threadObject_UI->idList->at(0), L"The service should have synced to the front end by now");
			}
			// sets up the threads like so:
			// Service -> App -> UI
			// then posts a property change to an object within the service thread object
			TEST_METHOD(ThreadObject_Observable_Object_ThreadObject)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }));
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }));
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ParentThreadObject>>(threadManager);
				auto childThreadObjectManager = std::make_shared<ThreadObjectManager<ExampleThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				// set up a waiter until we get a notification on the UI thread that something has been completed
				ReadyEvent e;
				auto subscribeHandle = threadObject_UI->exampleObject->subscribe([&e](std::shared_ptr<ExampleThreadObject> newValue)
				{
					e.finished();
				});

				auto newChildObject = childThreadObjectManager->getOrCreateObject(ThreadIds::Service, 4);
				newChildObject->name->set(L"new name");
				threadObject_Service->exampleObject->set(newChildObject);
				e.wait();

				Assert::IsTrue(nullptr != threadObject_UI->exampleObject->get(), L"Make sure the UI object was set");
				Assert::IsTrue(ThreadIds::UI == threadObject_UI->exampleObject->get()->getThreadId(), L"Expected the example object to be running on the UI thread");
				Assert::IsTrue(ThreadIds::Service == threadObject_Service->exampleObject->get()->getThreadId(), L"Expected the example object to be running on the Service thread");
				Assert::AreEqual(threadObject_UI->exampleObject->get()->getId(), threadObject_Service->exampleObject->get()->getId(), L"Make sure they have the same value");
				Assert::IsTrue(threadObject_UI->exampleObject->get().get() != threadObject_Service->exampleObject->get().get(), L"Make sure they are two distinct pointers");
				Assert::AreEqual(threadObject_UI->exampleObject->get()->name->get(), threadObject_Service->exampleObject->get()->name->get(), L"Make sure they have the same name value");
			}
			// sets up the threads like so:
			// Service -> App -> UI
			// then posts a property change to an object within the service thread object
			TEST_METHOD(ThreadObject_Observable_Vector_ThreadObject)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }));
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }));
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ParentThreadObject>>(threadManager);
				auto childThreadObjectManager = std::make_shared<ThreadObjectManager<ExampleThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				// set up a waiter until we get a notification on the UI thread that something has been completed
				ReadyEvent e;
				auto subscribeHandle = threadObject_UI->exampleObjects->subscribe([&e](std::shared_ptr<ExampleThreadObject> newValue, size_t index, ObservableActionType action)
				{
					e.finished();
				});

				auto sizeBefore = threadObject_UI->exampleObjects->size();

				auto newChildObject = childThreadObjectManager->getOrCreateObject(ThreadIds::Service, 4);
				newChildObject->name->set(L"new name");
				threadObject_Service->exampleObjects->insert(0, newChildObject);
				e.wait();

				Assert::IsTrue(nullptr != threadObject_UI->exampleObjects->at(0), L"Make sure the UI object was set");
				Assert::IsTrue(threadObject_UI->exampleObjects->size() == sizeBefore + 1, L"Expect example objects list to be size of 1");
				Assert::IsTrue(ThreadIds::UI == threadObject_UI->exampleObjects->at(0)->getThreadId(), L"Expected the example object to be running on the UI thread");
				Assert::IsTrue(ThreadIds::Service == threadObject_Service->exampleObjects->at(0)->getThreadId(), L"Expected the example object to be running on the Service thread");
				Assert::AreEqual(threadObject_UI->exampleObjects->at(0)->getId(), threadObject_Service->exampleObjects->at(0)->getId(), L"Make sure they have the same value");
				Assert::IsTrue(threadObject_UI->exampleObjects->at(0).get() != threadObject_Service->exampleObjects->at(0).get(), L"Make sure they are two distinct pointers");
				Assert::AreEqual(threadObject_UI->exampleObjects->at(0)->name->get(), threadObject_Service->exampleObjects->at(0)->name->get(), L"Make sure they have the same name value");
			}
		};
	}
}