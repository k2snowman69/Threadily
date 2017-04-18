#pragma once
#include <stdexcept>
struct ThreadObjectId {
	unsigned int id;

	ThreadObjectId() {
		this->id = 0;
	}
	ThreadObjectId(const ThreadObjectId& other) {
		this->id = other.id;
	}
	ThreadObjectId(unsigned int id) {
		this->id = id;
	}
	virtual operator unsigned int&() { return this->id; }
	virtual operator unsigned int() const { return this->id; }

	virtual bool operator < (const ThreadObjectId& other) {
		auto temp = dynamic_cast<const ThreadObjectId &>(other);
		// TODO throw exception
		return this->id < temp.id;
	};
};