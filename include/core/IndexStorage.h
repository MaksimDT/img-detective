#pragma once

#include "core/IndexNode.h"
#include "core/IFeatureDeserializer.h"

namespace ImgDetective {
namespace Core {

	ABSTRACT NONCOPYABLE class IndexStorage {
	public:
		#pragma region NESTED

		ABSTRACT NONCOPYABLE NESTED class LookupSessionBase {
		public:
			virtual ~LookupSessionBase() {};

			virtual bool GetNextPacket(REF IndexNode::col_p_t& packet) = 0;
		protected:
			CTOR LookupSessionBase(const REF IFeatureDeserializer& featureDeserializer);
			
			const REF IFeatureDeserializer& featureDeserializer;
		private:
			COPYCTOR LookupSessionBase(const REF LookupSessionBase& other);
		};

		#pragma endregion

		virtual ~IndexStorage();

		virtual LookupSessionBase* StartLookup() const = 0;
	protected:
		CTOR IndexStorage(EXCLUSIVE IFeatureDeserializer* featureDeserializer);

		EXCLUSIVE IFeatureDeserializer* featureDeserializer;
	private:
		COPYCTOR IndexStorage(const REF IndexStorage& other);
	};

}
}