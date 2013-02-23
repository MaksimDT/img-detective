#pragma once

#include "IndexNode.h"
#include "IFeatureDeserializer.h"

namespace ImgDetective {
namespace Core {

	ABSTRACT class IndexStorage {
	public:
		ABSTRACT NESTED class LookupSessionBase {
		public:
			virtual bool GetNextPacket(REF IndexNode::col_p_t& packet) = 0;
		protected:
			LookupSessionBase(const REF IFeatureDeserializer& featureDeserializer);
			virtual ~LookupSessionBase();

			const REF IFeatureDeserializer& featureDeserializer;
		};

		virtual LookupSessionBase* StartLookup() const = 0;
	protected:
		IndexStorage(IFeatureDeserializer* featureDeserializer);
		virtual ~IndexStorage();

		IFeatureDeserializer* featureDeserializer;
	};

}
}