#pragma once

#include "core/IndexNode.h"
#include "core/IFeatureDeserializer.h"

namespace ImgDetective {
namespace Core {

    //acts as a driver for exhanging info with a persistent medium where features of images are stored
    INTERFACE class IIndexStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        #pragma region NESTED
        
        INTERFACE class ILookupSession : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
        public:
            CTOR ILookupSession() {};
            virtual bool GetNextPacket(REF IndexNode::col_p_t& packet) = 0;
        };

        #pragma endregion

        CTOR IIndexStorage() {};
        virtual EXCLUSIVE ILookupSession* StartLookup() const = 0;
        virtual void AddFeature(const REF IFeature& feature, imgid_t imgId);
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