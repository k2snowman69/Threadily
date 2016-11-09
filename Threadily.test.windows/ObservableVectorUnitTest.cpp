#include "stdafx.h"
#include "CppUnitTest.h"

#include "ThreadIds.h"

#include <Observable.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace threadily
{
	namespace test
	{
		TEST_CLASS(ObservableVectorUnitTest)
		{
		public:

			TEST_METHOD(Observable_Vector_Int_Insert_1)
			{
				auto observableVector = Observable<std::vector<int>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.insert(0, 1);
				observableVector.insert(1, 2);
				observableVector.insert(0, 0);
				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array");

				Assert::AreEqual(0, observableVector.at(0));
				Assert::AreEqual(1, observableVector.at(1));
				Assert::AreEqual(2, observableVector.at(2));
			}

			TEST_METHOD(Observable_Vector_Int_Insert_2)
			{
				auto observableVector = Observable<std::vector<int>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.insert(2, 2);
				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty

				Assert::AreEqual(0, observableVector.at(0));
				Assert::AreEqual(0, observableVector.at(1));
				Assert::AreEqual(2, observableVector.at(2));
			}

			TEST_METHOD(Observable_Vector_Int_Set)
			{
				auto observableVector = Observable<std::vector<int>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.insert(2, 2);
				observableVector.set(1, 1);
				observableVector.set(0, 0);

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty
				Assert::AreEqual(0, observableVector.at(0));
				Assert::AreEqual(1, observableVector.at(1));
				Assert::AreEqual(2, observableVector.at(2));
			}

			TEST_METHOD(Observable_Vector_Int_Set_OutOfOrder)
			{
				auto observableVector = Observable<std::vector<int>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.set(2, 2);
				observableVector.set(1, 1);
				observableVector.set(0, 0);

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty
				Assert::AreEqual(0, observableVector.at(0));
				Assert::AreEqual(1, observableVector.at(1));
				Assert::AreEqual(2, observableVector.at(2));
			}
			TEST_METHOD(Observable_Vector_Ptr_Insert_1)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.insert(0, std::make_shared<int>(1));
				observableVector.insert(1, std::make_shared<int>(2));
				observableVector.insert(0, std::make_shared<int>(0));
				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array");

				Assert::AreEqual(0, *(observableVector.at(0).get()));
				Assert::AreEqual(1, *(observableVector.at(1).get()));
				Assert::AreEqual(2, *(observableVector.at(2).get()));
			}

			TEST_METHOD(Observable_Vector_Ptr_Insert_2)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.insert(2, std::make_shared<int>(2));
				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty

				Assert::IsNull(observableVector.at(0).get());
				Assert::IsNull(observableVector.at(1).get());
				Assert::AreEqual(2, *(observableVector.at(2).get()));
			}

			TEST_METHOD(Observable_Vector_Ptr_Set)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.insert(2, std::make_shared<int>(2));
				observableVector.set(1, std::make_shared<int>(1));
				observableVector.set(0, std::make_shared<int>(0));

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty
				Assert::AreEqual(0, *(observableVector.at(0).get()));
				Assert::AreEqual(1, *(observableVector.at(1).get()));
				Assert::AreEqual(2, *(observableVector.at(2).get()));
			}

			TEST_METHOD(Observable_Vector_Ptr_Set_OutOfOrder)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				observableVector.set(2, std::make_shared<int>(2));
				observableVector.set(1, std::make_shared<int>(1));
				observableVector.set(0, std::make_shared<int>(0));

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty
				Assert::AreEqual(0, *(observableVector.at(0).get()));
				Assert::AreEqual(1, *(observableVector.at(1).get()));
				Assert::AreEqual(2, *(observableVector.at(2).get()));
			}

			TEST_METHOD(Observable_Vector_Ptr_Subscription_Insert)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				auto valueAdded = std::make_shared<int>(4);

				auto subscribe = observableVector.subscribe([valueAdded](std::shared_ptr<int> newValue, size_t index, ObservableActionType action) {
					Assert::IsTrue(valueAdded == newValue, L"Value is correct");
					Assert::AreEqual((size_t)2, index, L"index is correct");
					Assert::IsTrue(ObservableActionType::Insert == action, L"action is correct");
				});

				observableVector.insert(2, valueAdded);

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array"); // 1 real, 2 empty

				observableVector.unsubscribe(subscribe);
			}

			TEST_METHOD(Observable_Vector_Ptr_Subscription_Remove)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				bool isDeleted = false;
				auto valueAdded = std::make_shared<int>(4);

				auto subscribe = observableVector.subscribe([valueAdded, &isDeleted](std::shared_ptr<int> newValue, size_t index, ObservableActionType action) {
					if (ObservableActionType::Erase == action)
					{
						Assert::IsTrue(valueAdded == newValue, L"Value is correct");
						Assert::AreEqual((size_t)2, index, L"index is correct");
						isDeleted = true;
					}
				});

				observableVector.set(0, std::make_shared<int>(0));
				observableVector.set(1, std::make_shared<int>(2));
				observableVector.set(2, valueAdded);

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array");

				observableVector.erase(2);

				Assert::AreEqual((size_t)2, observableVector.size(), L"Expect values in the array");
				Assert::IsTrue(isDeleted);

				observableVector.unsubscribe(subscribe);
			}

			TEST_METHOD(Observable_Vector_Ptr_Subscription_Update)
			{
				auto observableVector = Observable<std::vector<std::shared_ptr<int>>>();
				Assert::AreEqual((size_t)0, observableVector.size(), L"Expect no values in the array at the beginning");

				bool isUpdated = false;
				auto valueAdded = std::make_shared<int>(4);
				auto valueUpdated = std::make_shared<int>(6);

				auto subscribe = observableVector.subscribe([valueUpdated, &isUpdated](std::shared_ptr<int> newValue, size_t index, ObservableActionType action) {
					if (ObservableActionType::Set == action)
					{
						Assert::IsTrue(valueUpdated == newValue, L"Value is correct");
						Assert::AreEqual((size_t)2, index, L"index is correct");
						Assert::IsTrue(ObservableActionType::Set == action, L"action is correct");
						isUpdated = true;
					}
				});

				observableVector.set(0, std::make_shared<int>(0));
				observableVector.set(1, std::make_shared<int>(2));
				observableVector.set(2, valueAdded);

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array");

				observableVector.set(2, valueUpdated);

				Assert::AreEqual((size_t)3, observableVector.size(), L"Expect values in the array");
				Assert::IsTrue(isUpdated);

				observableVector.unsubscribe(subscribe);
			}
		};
	}
}