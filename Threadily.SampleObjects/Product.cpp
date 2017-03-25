#include "Product.h"

#include <ThreadManager.h>
#include <ThreadObjectManager.h>

#include "ProductManager.h"
#include "BusinessManager.h"

#ifdef EMSCRIPTEN
#include <ThreadObjectManager.h>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
	namespace test {
		Product::Product(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id)
			: ThreadObject(objectManager, threadId, id)
		{
			this->name = std::make_shared<threadily::Observable<std::string>>();
		}
		Product::~Product()
		{
		}
		std::vector<std::shared_ptr<threadily::IObservable>> Product::getObservableProperties()
		{
			return std::vector<std::shared_ptr<threadily::IObservable>>({
				name,
			});
		}

#ifdef EMSCRIPTEN

		defineThreadObjectCallbackHandler(Product, Product);
		defineThreadObjectCallbackVectorHandler(Product, Product);

		EMSCRIPTEN_BINDINGS(Product)
		{

			defineThreadObjectObservable(Product, Product);
			defineThreadObjectObservableVector(Product, Product);

			class_<Product, base<threadily::ThreadObject>>("Product")
				.smart_ptr<std::shared_ptr<Product>>("Product")
				.property("name", &Product::name)
				;
		}
#endif
	}
}
