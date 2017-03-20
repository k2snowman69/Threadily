#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include <Observable.h>

#include <ThreadManager.h>
#include <ThreadObjectManager.h>
#include <ThreadablesThreadObject.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ThreadablesThreadObjectUnitTest)
		{
		public:

			TEST_METHOD(ThreadablesThreadObject_Empty)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				auto uiThread = threadManager->getOrCreateThread(ThreadIds::ThreadId_UI);
				auto appThread = threadManager->getOrCreateThread(ThreadIds::ThreadId_App, std::set<unsigned int>({ ThreadIds::ThreadId_UI }), nullptr);
				auto serviceThread = threadManager->getOrCreateThread(ThreadIds::ThreadId_Service, std::set<unsigned int>({ ThreadIds::ThreadId_App }), nullptr);

				auto threadablesThreadObjectManager = std::make_shared<ThreadObjectManager<ThreadablesThreadObject>>(threadManager);
				auto emptyObjectManager = std::make_shared<ThreadObjectManager<EmptyThreadObject>>(threadManager);

				auto obj_Service = threadablesThreadObjectManager->getOrCreateObject(ThreadIds::ThreadId_Service, 0);
				auto obj_UI = threadablesThreadObjectManager->getOrCreateObject(ThreadIds::ThreadId_UI, 0);

				auto emptyObject = emptyObjectManager->getOrCreateObject(ThreadIds::ThreadId_Service, 0);

				auto handler = obj_UI->emptyObject->subscribe([emptyObject](std::shared_ptr<EmptyThreadObject> newValue){
					Assert::AreEqual(emptyObject->getId(), newValue->getId());
				});

				Assert::IsNull(obj_UI->emptyObject->get().get());
				obj_Service->emptyObject->set(emptyObject);

				obj_UI->emptyObject->unsubscribe(handler);
			}
		};
	}
}