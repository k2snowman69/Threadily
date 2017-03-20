#include "stdafx.h"
#include "CppUnitTest.h"

#include <ThreadManager.h>
#include <ThreadQueueItem.h>
#include <ReadyEvent.h>

#include "ThreadIds.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ReadyEventUnitTest)
		{
		public:

			TEST_METHOD(ReadyEvent_FinishedThenWait)
			{
				ReadyEvent re;

				re.finished();
				re.wait();

				// if the test didn't time out then we are successful
			}

			TEST_METHOD(ReadyEvent_WaitThenFinished)
			{
				auto threadManager = std::make_shared<ThreadManager>();
				auto uiThread = threadManager->getOrCreateThread(ThreadIds::ThreadId_UI);
				auto appThread = threadManager->getOrCreateThread(ThreadIds::ThreadId_App);

				ReadyEvent re;
				bool isFinished = false;

				uiThread->addWork(std::make_shared<ThreadQueueItem>([&re, &isFinished]() {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					isFinished = true;
					re.finished();
				}));
				re.wait();

				Assert::IsTrue(isFinished, L"Thread work finished before hitting end of method");
			}
		};
	}
}