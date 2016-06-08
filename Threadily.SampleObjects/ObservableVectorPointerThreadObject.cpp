#include "ObservableVectorPointerThreadObject.h"

namespace threadily
{
	namespace test
	{
		ObservableVectorPointerThreadObject::ObservableVectorPointerThreadObject(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id) :
			ThreadObject(objectManager, threadId, id)
		{
			idList = std::make_shared<Observable<std::vector<int>>>();
		}

		std::vector<std::shared_ptr<IObservable>> ObservableVectorPointerThreadObject::getObservableProperties()
		{
			return std::vector<std::shared_ptr<IObservable>>({ idList });
		}
	}
}