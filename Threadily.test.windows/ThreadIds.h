#pragma once

namespace threadily
{
	namespace test
	{
		// Threads are listed in the order of previous -> next notifications. Please keep this order
		enum ThreadIds : unsigned int
		{
			Test = 0,
			Service = 1,
			App = 2,
			UI = 3
		};
	}
}