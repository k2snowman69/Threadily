#pragma once

#include <IThreadObjectId.h>

namespace threadily {
	namespace test {
		struct ProductId : public IThreadObjectId {
			unsigned int businessId;
			unsigned int productId;
			ProductId()
			{
				this->businessId = 0;
				this->productId = 0;
			}
			ProductId(unsigned int businessId, unsigned int productId)
			{
				this->businessId = businessId;
				this->productId = productId;
			}
			// Copy constructor
			ProductId(const ProductId& other) {
				this->businessId = other.businessId;
				this->productId = other.productId;
			}
			// Override < so that comparisons for maps work
			bool operator < (const ProductId& other) const {
				return 
					// First if the businesses are null then do the products
					(this->businessId == other.businessId && this->productId < other.productId) ||
					// if not just do the businesses
					this->businessId < other.businessId;
			};
		};
	}
}