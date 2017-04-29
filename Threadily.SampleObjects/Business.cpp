#include "Business.h"

#include <math.h>

#include <ThreadManager.h>
#include <ThreadObjectManager.h>

#include "BusinessManager.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
	namespace test {
#pragma region Constructor/Threadily
		Business::Business(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, const ThreadObjectId & id)
			: ThreadObject(objectManager, threadId, id)
		{
			this->name = std::make_shared<threadily::Observable<std::string>>();
			// CreateProduct
			this->isCreateProductPending = std::make_shared<threadily::Observable<bool>>();
			this->isCreateProductPending->set(false);
			this->createdProduct = std::make_shared<threadily::Observable<Product>>(threadId);
			// Read Products
			this->isProductsPending = std::make_shared<threadily::Observable<bool>>();
			this->products = std::make_shared<threadily::Observable<std::vector<Product>>>(threadId);

			this->productCounter = 0;
		}
		Business::~Business()
		{
		}
		std::vector<std::shared_ptr<threadily::IObservable>> Business::getObservableProperties()
		{
			return std::vector<std::shared_ptr<threadily::IObservable>>({
				name,
				isCreateProductPending,
				createdProduct,
				isProductsPending,
				products
			});
		}
#pragma endregion
#pragma region Product
		void Business::createProductAsync(std::string name)
		{
			// if this isn't the service thread, then put the work on the service thread
			if (this->getThreadId() != ThreadIds::ThreadId_Service)
			{
				this->runOnPeer(ThreadIds::ThreadId_Service, [name](std::shared_ptr<IThreadObject> sibling) {
					auto serviceObject = std::static_pointer_cast<Business>(sibling);
					serviceObject->createProductAsync(name);
				});
				return;
			}
			
			this->isCreateProductPending->set(true);
			// create the new Product object
			auto objectManager = this->getObjectManager().lock();
			if (objectManager == nullptr)
			{
				return;
			}

			auto threadilyResult = std::static_pointer_cast<BusinessManager>(objectManager)->getProductManager()->getOrCreateObject(ThreadIds::ThreadId_Service, this->getId(), productCounter++);
			threadilyResult->name->set(name);
			this->createdProduct->set(threadilyResult);

			// set the property to no longer pending
			this->isCreateProductPending->set(false);
		}
		void Business::readProductsAsync(unsigned int index, unsigned int count, std::string name)
		{
			// if this isn't the service thread, then put the work on the service thread
			if (this->getThreadId() != ThreadIds::ThreadId_Service)
			{
				auto d = this->shared_from_this();

				this->runOnPeer(ThreadIds::ThreadId_Service, [index, count, name](std::shared_ptr<IThreadObject> sibling) {
					auto serviceObject = std::static_pointer_cast<Business>(sibling);
					serviceObject->readProductsAsync(index, count, name);
				});
				return;
			}

			this->isProductsPending->set(true);
			// If this response doesn't match the latest name requested, we should exit

			auto objectManager = this->getObjectManager().lock();
			if (objectManager == nullptr)
			{
				return;
			}
			auto manager = std::static_pointer_cast<BusinessManager>(objectManager)->getProductManager();

			// go through the results and populate the query value
			for (unsigned int id = index; (id < index + count) && (id < this->productCounter); id++)
			{
				auto threadilyResult = manager->getOrCreateObject(ThreadIds::ThreadId_Service, this->getId(), id);
				this->products->set(id, threadilyResult);
			}

			// set the property to no longer pending
			this->isProductsPending->set(false);
		}
#pragma endregion

#ifdef EMSCRIPTEN

		defineThreadObjectCallbackHandler(Business, Business);
		defineThreadObjectCallbackVectorHandler(Business, Business);

		EMSCRIPTEN_BINDINGS(Business)
		{

			defineThreadObjectObservable(Business, Business);
			defineThreadObjectObservableVector(Business, Business);

			class_<Business, base<threadily::ThreadObject<>>>("Business")
				.smart_ptr<std::shared_ptr<Business>>("Business")
				.property("name", &Business::name)
				.property("isCreateProductPending", &Business::isCreateProductPending)
				.property("createdProduct", &Business::createdProduct)
				.property("isProductsPending", &Business::isProductsPending)
				.property("products", &Business::products)
				.function("readProductsAsync", &Business::readProductsAsync)
				.function("createProductAsync", &Business::createProductAsync)
				;
		}
#endif
	}
}
