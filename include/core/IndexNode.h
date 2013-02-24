#pragma once

#include <vector>
#include "common/CommonInternal.h"
#include "core/Feature.h"

namespace ImgDetective {
namespace Core {

	struct IndexNode {
		typedef std::vector<IndexNode*> col_p_t;
		imgid_t imgId;
		Feature* feature;
	};

}
}