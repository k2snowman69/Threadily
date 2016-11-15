#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include "ThreadManager.h"
#include "ThreadObjectManager.h"
#include "ThreadObject.h"
#include "PrimativesThreadObject.h"
#include "ReadyEvent.h"
#include "ThreadablesThreadObject.h"

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

				auto threadObjectManager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);
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

				threadObject_UI->name->unsubscribe(subscribeHandle);
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

				auto threadObjectManager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				// set up a waiter until we get a notification on the UI thread that something has been completed
				ReadyEvent e;
				auto subscribeHandle = threadObject_UI->intArray->subscribe([&e](int newValue, size_t index, ObservableActionType action)
				{
					e.finished();
				});

				threadObject_Service->intArray->set(0, 17);
				e.wait();

				Assert::AreEqual((size_t)1, threadObject_Service->intArray->size(), L"We directly added an element to service idList");
				Assert::AreEqual((size_t)1, threadObject_UI->intArray->size(), L"The service should have synced to the front end by now");

				Assert::AreEqual(17, threadObject_Service->intArray->at(0), L"We directly added an element to service idList");
				Assert::AreEqual(17, threadObject_UI->intArray->at(0), L"The service should have synced to the front end by now");

				threadObject_UI->intArray->unsubscribe(subscribeHandle);
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

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ThreadablesThreadObject>>(threadManager);
				auto childThreadObjectManager = std::make_shared<ThreadObjectManager<EmptyThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				// set up a waiter until we get a notification on the UI thread that something has been completed
				ReadyEvent e;
				auto subscribeHandle = threadObject_UI->emptyObject->subscribe([&e](std::shared_ptr<EmptyThreadObject> newValue)
				{
					e.finished();
				});

				auto newChildObject = childThreadObjectManager->getOrCreateObject(ThreadIds::Service, 4);
				threadObject_Service->emptyObject->set(newChildObject);
				e.wait();

				Assert::IsTrue(nullptr != threadObject_UI->emptyObject->get(), L"Make sure the UI object was set");
				Assert::IsTrue(ThreadIds::UI == threadObject_UI->emptyObject->get()->getThreadId(), L"Expected the example object to be running on the UI thread");
				Assert::IsTrue(ThreadIds::Service == threadObject_Service->emptyObject->get()->getThreadId(), L"Expected the example object to be running on the Service thread");
				Assert::AreEqual(threadObject_UI->emptyObject->get()->getId(), threadObject_Service->emptyObject->get()->getId(), L"Make sure they have the same value");
				Assert::IsTrue(threadObject_UI->emptyObject->get().get() != threadObject_Service->emptyObject->get().get(), L"Make sure they are two distinct pointers");
				Assert::AreEqual(threadObject_UI->emptyObject->get()->getId(), threadObject_Service->emptyObject->get()->getId(), L"Make sure they have the same id value");

				threadObject_UI->emptyObject->unsubscribe(subscribeHandle);
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

				auto threadObjectManager = std::make_shared<ThreadObjectManager<ThreadablesThreadObject>>(threadManager);
				auto childThreadObjectManager = std::make_shared<ThreadObjectManager<EmptyThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				// set up a waiter until we get a notification on the UI thread that something has been completed
				ReadyEvent e;
				auto subscribeHandle = threadObject_UI->emptyObjectArray->subscribe([&e](std::shared_ptr<EmptyThreadObject> newValue, size_t index, ObservableActionType action)
				{
					e.finished();
				});

				auto sizeBefore = threadObject_UI->emptyObjectArray->size();

				auto newChildObject = childThreadObjectManager->getOrCreateObject(ThreadIds::Service, 4);
				threadObject_Service->emptyObjectArray->insert(0, newChildObject);
				e.wait();

				Assert::IsTrue(nullptr != threadObject_UI->emptyObjectArray->at(0), L"Make sure the UI object was set");
				Assert::IsTrue(threadObject_UI->emptyObjectArray->size() == sizeBefore + 1, L"Expect example objects list to be size of 1");
				Assert::IsTrue(ThreadIds::UI == threadObject_UI->emptyObjectArray->at(0)->getThreadId(), L"Expected the example object to be running on the UI thread");
				Assert::IsTrue(ThreadIds::Service == threadObject_Service->emptyObjectArray->at(0)->getThreadId(), L"Expected the example object to be running on the Service thread");
				Assert::AreEqual(threadObject_UI->emptyObjectArray->at(0)->getId(), threadObject_Service->emptyObjectArray->at(0)->getId(), L"Make sure they have the same value");
				Assert::IsTrue(threadObject_UI->emptyObjectArray->at(0).get() != threadObject_Service->emptyObjectArray->at(0).get(), L"Make sure they are two distinct pointers");
				Assert::AreEqual(threadObject_UI->emptyObjectArray->at(0)->getId(), threadObject_Service->emptyObjectArray->at(0)->getId(), L"Make sure they have the same name value");

				threadObject_UI->emptyObjectArray->unsubscribe(subscribeHandle);
			}
			// sets up the threads like so:
			// Service -> App -> UI
			// And checks to make sure the initial values of each object are the same
			TEST_METHOD(ThreadObject_Observable_InitialDefaultValue)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::Service, std::set<unsigned int>({ ThreadIds::App }));
				threadManager->getOrCreateThread(ThreadIds::App, std::set<unsigned int>({ ThreadIds::UI }));
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_Service = threadObjectManager->getOrCreateObject(ThreadIds::Service, 0);

				Assert::AreEqual(threadObject_UI->intValue->get(), threadObject_Service->intValue->get(), L"UI and service have the same value");

				auto threadObject_App = threadObjectManager->getOrCreateObject(ThreadIds::App, 0);

				Assert::AreEqual(threadObject_UI->intValue->get(), threadObject_App->intValue->get(), L"UI and service have the same value");
			}
			TEST_METHOD(ThreadObject_RunOnPeer_Success)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				threadManager->getOrCreateThread(ThreadIds::App);
				threadManager->getOrCreateThread(ThreadIds::UI);

				auto threadObjectManager = std::make_shared<ThreadObjectManager<PrimativesThreadObject>>(threadManager);
				auto threadObject_UI = threadObjectManager->getOrCreateObject(ThreadIds::UI, 0);
				auto threadObject_App = threadObjectManager->getOrCreateObject(ThreadIds::App, 0);

				threadObject_UI->intValue->set(0);
				threadObject_App->intValue->set(0);
				ReadyEvent e;
				threadObject_UI->runOnPeer(ThreadIds::App, [&e](std::shared_ptr<IThreadObject> peer) {
					auto appObject = std::static_pointer_cast<PrimativesThreadObject>(peer);
					appObject->intValue->set(4);
					e.finished();
				});

				e.wait();
				Assert::AreEqual(4, threadObject_App->intValue->get());
			}
		};
	}
}