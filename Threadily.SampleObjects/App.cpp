#include "App.h"

#include <ThreadManager.h>
#include <ThreadObjectManager.h>

#include "AppManager.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
	namespace test {
#pragma region Constructor/Threadily
		App::App(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id)
			: ThreadObject(objectManager, threadId, id)
		{
			// Business 
			this->isCreateBusinessPending = std::make_shared<threadily::Observable<bool>>();
			this->isCreateBusinessPending->set(false);
			this->createdBusiness = std::make_shared<threadily::Observable<Business>>(threadId);

			this->isBusinessesPending = std::make_shared<threadily::Observable<bool>>();
			this->businesses = std::make_shared<threadily::Observable<std::vector<Business>>>(threadId);

			this->businessCounter = 0;
		}
		App::~App()
		{
		}
		std::vector<std::shared_ptr<threadily::IObservable>> App::getObservableProperties()
		{
			return std::vector<std::shared_ptr<threadily::IObservable>>({
				isCreateBusinessPending,
				createdBusiness,
				isBusinessesPending,
				businesses,
			});
		}
#pragma endregion
#pragma region Business
		void App::createBusinessAsync(std::string name)
		{
			// if this isn't the service thread, then put the work on the service thread
			if (this->getThreadId() != ThreadIds::ThreadId_Service)
			{
				this->runOnPeer(ThreadIds::ThreadId_Service, [name](std::shared_ptr<IThreadObject> sibling) {
					auto serviceObject = std::static_pointer_cast<App>(sibling);
					serviceObject->createBusinessAsync(name);
				});
				return;
			}

			this->isCreateBusinessPending->set(true);

			auto objectManager = this->getObjectManager().lock();
			if (objectManager == nullptr)
			{
				return;
			}

			auto threadilyResult = std::static_pointer_cast<AppManager>(objectManager)->getBusinessManager()->getOrCreateObject(ThreadIds::ThreadId_Service, this->businessCounter++);
			threadilyResult->name->set(name);
			this->createdBusiness->set(threadilyResult);

			// set the property to no longer pending
			this->isCreateBusinessPending->set(false);
		}
		void App::readBusinessesAsync(unsigned int index, unsigned int count, std::string name)
		{
			// if this isn't the service thread, then put the work on the service thread
			if (this->getThreadId() != ThreadIds::ThreadId_Service)
			{
				auto d = this->shared_from_this();

				this->runOnPeer(ThreadIds::ThreadId_Service, [this, index, count, name](std::shared_ptr<IThreadObject> sibling) {
					auto serviceObject = std::static_pointer_cast<App>(sibling);
					serviceObject->readBusinessesAsync(index, count, name);
				});
				return;
			}

			this->isBusinessesPending->set(true);

			auto objectManager = this->getObjectManager().lock();
			if (objectManager == nullptr)
			{
				return;
			}
			auto manager = std::static_pointer_cast<AppManager>(objectManager)->getBusinessManager();

			// go through the results and populate the query value
			for (unsigned int id = index; (id < index + count) && (id < this->businessCounter); id++)
			{
				auto threadilyResult = manager->getOrCreateObject(ThreadIds::ThreadId_Service, id);
				this->businesses->set(id, threadilyResult);
			}

			// set the property to no longer pending
			this->isBusinessesPending->set(false);
		}
		void App::throwTest1()
		{
			throw new std::runtime_error("throwTest1");
		}
		void App::throwTest2()
		{
			throw std::runtime_error("throwTest1");
		}
		void App::throwTest3()
		{
			throw "throwTest1";
		}
#pragma endregion 

#ifdef EMSCRIPTEN

		EMSCRIPTEN_BINDINGS(App)
		{
			class_<App, base<threadily::ThreadObject<>>>("App")
				.smart_ptr<std::shared_ptr<App>>("App")
				.property("isBusinessesPending", &App::isBusinessesPending)
				.property("businesses", &App::businesses)
				.property("isCreateBusinessPending", &App::isCreateBusinessPending)
				.property("createdBusiness", &App::createdBusiness)
				.function("createBusinessAsync", &App::createBusinessAsync)
				.function("readBusinessesAsync", &App::readBusinessesAsync)
				.function("throwTest1", &App::throwTest1)
				.function("throwTest2", &App::throwTest2)
				.function("throwTest3", &App::throwTest3)
				;
		}
#endif
	}
}
