#pragma once

namespace threadily {
	// Subclasses need to implement
	// - Copy constructor
	struct IThreadObjectId {
		// Required for map comparisons
		virtual bool operator < (const IThreadObjectId& other) const {
			throw std::runtime_error("Need to override operator <");
		};
	};
}