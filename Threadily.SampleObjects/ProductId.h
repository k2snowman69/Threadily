#pragma once

#include <ThreadObjectId.h>

struct ProductId : ThreadObjectId {
	unsigned int instanceCount;
	unsigned int businessId;
	unsigned int productId;
	ProductId() {
		instanceCount = 0;
		businessId = 0;
		productId = 0;
	}
	ProductId(unsigned int instanceCount, unsigned int businessId, unsigned int productId)
		: businessId(businessId),
		productId(productId),
		ThreadObjectId(instanceCount)
	{}
};