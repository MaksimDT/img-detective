#pragma once

#include <vector>

namespace ImgDetective {
namespace Core {

	class Feature {
	public:
		typedef int type_id_t;
		/*type of collection that should be used to transfer objects of this type across 
		methods boundaries (i.e. for params and return values). "p" means that elements 
		of collection is pointers*/
		typedef std::vector<Feature*> col_p_t;

		type_id_t GetTypeId() const;
		double CompareToOther(Feature& feature) const;
	private:
		type_id_t typeId;
	};
}
}