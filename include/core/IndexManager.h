#pragma once

#include "common/CommonInternal.h"
#include "core/Feature.h"
#include "core/IndexSeekResult.h"
#include "core/IndexStorage.h"

namespace ImgDetective {
namespace Core {

    INTERFACE class IIndexManager : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        typedef std::vector<IIndexManager*> col_p_t;

        virtual IndexSeekResult* Search(REF IFeature& f, const REF ImgQuery& query) const = 0;
        virtual IFeature::type_id_t GetFeatureTypeId() const = 0;
        virtual void AddFeature(REF IFeature& feature, imgid_t imgId) = 0;
    };

	//contains logic of sorting features by similarity to feature of query image
    ABSTRACT class IndexManager : public IIndexManager {
	public:
		virtual ~IndexManager();
		virtual IFeature::type_id_t GetFeatureTypeId() const;
        virtual void AddFeature(REF IFeature& feature, imgid_t imgId);
	protected:
		CTOR IndexManager(EXCLUSIVE IIndexStorage* storage, IFeature::type_id_t featureTypeId);
		
		EXCLUSIVE IIndexStorage* storage;
	private:
		IFeature::type_id_t featureTypeId;
	};

}
}