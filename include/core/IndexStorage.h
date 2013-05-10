#pragma once

#include "core/IndexNode.h"
#include "core/IFeatureDeserializer.h"

namespace ImgDetective {
namespace Core {

    //acts as a driver for exhanging info with a persistent medium where features of images are stored
    class IIndexStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        #pragma region NESTED
        
        class ILookupSession : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
        public:
            CTOR ILookupSession() {};
            virtual IndexNode::col_t GetNextPacket() = 0;
        };

        #pragma endregion

        CTOR IIndexStorage() {};
        virtual EXCLUSIVE ILookupSession* StartLookup() const = 0;
        virtual void AddFeature(const REF IFeature& feature, imgid_t imgId) const = 0;
    };

	ABSTRACT class IndexStorage : public IIndexStorage {
	public:
		#pragma region NESTED

        ABSTRACT NESTED class LookupSessionBase : public ILookupSession {
		protected:
			CTOR LookupSessionBase(const REF IFeatureDeserializer& featureDeserializer);
			
			const REF IFeatureDeserializer& featureDeserializer;
		};

		#pragma endregion

        virtual ~IndexStorage();
	protected:
		CTOR IndexStorage(EXCLUSIVE IFeatureDeserializer* featureDeserializer);

		EXCLUSIVE IFeatureDeserializer* featureDeserializer;
	};

}
}