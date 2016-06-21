#pragma once

#include <memory>

#include "IThreadManager.h"
#include "IThreadObject.h"

namespace threadily {

	class IThreadObjectManager
	{
	public:
		virtual std::shared_ptr<IThreadManager> getThreadManager() = 0;
		virtual std::shared_ptr<IThreadObject> getPeer(unsigned int threadId, std::shared_ptr<IThreadObject> object) = 0;
	};

}