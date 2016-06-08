#pragma once

#include <memory>

namespace threadily {

	/// Represents work that needs to be executed upon a thread
	class IThreadQueueItem {
	public:
		/// Method that will run the actual work that needs to be completed
		virtual void run() = 0;

		/// Used by the caller to hold onto a reference of any ThreadWork if needed
		static std::shared_ptr<IThreadQueueItem> getReference(std::shared_ptr<IThreadQueueItem> value)
		{
			return value;
		}
	};
}