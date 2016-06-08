#pragma once

#include <memory>

#include "IThreadManager.h"

namespace threadily {

	class IThreadObjectManager
	{
	public:
		virtual std::shared_ptr<IThreadManager> getThreadManager() = 0;
	};

}