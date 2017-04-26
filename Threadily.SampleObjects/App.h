#pragma once

#include <Observable.h>
#include <IThreadManager.h>
#include <ThreadObjectId.h>
#include <ThreadObject.h>
#include <ThreadObjectManager.h>
#include <stack>

#include "ThreadIds.h"

#include "Business.h"
#include "Product.h"

namespace threadily {
	namespace test {
		class App : public threadily::ThreadObject<>
		{
		public:
			// Business
			std::shared_ptr<threadily::Observable<bool>> isCreateBusinessPending;
			std::shared_ptr<threadily::Observable<Business>> createdBusiness;

			std::shared_ptr<threadily::Observable<bool>> isBusinessesPending;
			std::shared_ptr<threadily::Observable<std::vector<Business>>> businesses;
		private:
			unsigned int businessCounter;
		public:
			App(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id);
			~App();
			virtual std::vector<std::shared_ptr<threadily::IObservable>> getObservableProperties() override;
			// Business
			void createBusinessAsync(std::string name);
			void readBusinessesAsync(unsigned int index, unsigned int count, std::string name);
			// Exception tests
			void throwTest1();
			void throwTest2();
			void throwTest3();
		};
	}
}
