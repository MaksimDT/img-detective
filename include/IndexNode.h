#pragma once

#include <vector>
#include <CommonInternal.h>
#include <Feature.h>

namespace ImgDetective {
namespace Core {

	typedef struct IndexNode {
		typedef std::vector<IndexNode*> col_p_t;
		imgid_t imgId;
		Feature* feature;
	} IndexNode;

}
}