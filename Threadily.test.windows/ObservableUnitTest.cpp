#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include <Observable.h>
#include <ReadyEvent.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ObservableUnitTest)
		{
		public:

			TEST_METHOD(Observable_Int_Insert_1)
			{
				auto observable = Observable<int>();
				Assert::AreEqual(0, observable.get(), L"Expect default value");

				observable.set(5);
				Assert::AreEqual(5, observable.get(), L"Expect set value");
			}
			// Bug fix - If we removed a subscription during callbacks, we would get an error
			TEST_METHOD(Observable_Int_SubscriptionBug_2017_02_26)
			{
				auto observable = Observable<int>();
				Assert::AreEqual(0, observable.get(), L"Expect default value");

				threadily::ReadyEvent r1, r2, r3;

				auto subscription = observable.subscribe([&r1](int newValue) {
					r1.finished();
				});
				auto subscription1 = observable.subscribe([&observable, &subscription, &r2](int newValue) {
					observable.unsubscribe(subscription);
					r2.finished();
				});
				auto subscription2 = observable.subscribe([&r3](int newValue) {
					r3.finished();
				});

				observable.set(5);
				r1.wait();
				r2.wait();
				r3.wait();

				Assert::AreEqual(5, observable.get(), L"Expect set value");
			}
			TEST_METHOD(Observable_Ptr_Int_Insert_1)
			{
				auto observable = Observable<std::shared_ptr<int>>();
				Assert::IsTrue(nullptr == observable.get(), L"Expect default value");

				observable.set(std::make_shared<int>(5));
				Assert::IsTrue(nullptr != observable.get(), L"");
				Assert::AreEqual(5, *observable.get(), L"Expect set value");
			}
		};
	}
}