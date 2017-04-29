#pragma once

#include "ThreadIds.h"

#include "App.h"

namespace threadily {
	namespace test {
		class AppFactory
		{
		public:
			static AppFactory& getInstance()
			{
				static AppFactory instance; // Guaranteed to be destroyed.
										// Instantiated on first use.
				return instance;
			};
		private:
			AppFactory();
			std::shared_ptr<threadily::ThreadObjectManager<App>> appThreadObjectManager;
			size_t instanceCount;
		public:
			std::shared_ptr<App> create();
		};
	}
}