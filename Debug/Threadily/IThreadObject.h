#pragma once

#include <memory>
#include <vector>
#include "IObservable.h"

namespace threadily {

	class IThreadObject
	{
	public:
		virtual unsigned int getThreadId() = 0;
		virtual unsigned int getId() = 0;
		virtual std::vector<std::shared_ptr<IObservable>> getObservableProperties() = 0;
	};

}
