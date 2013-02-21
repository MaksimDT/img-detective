#pragma once

#include "IndexNode.h"

namespace ImgDetective {
namespace Core {

	class IIndexStorage {
	public:
		virtual IndexNode::col_p_t GetNextPacket(unsigned int packetSize, unsigned int packetNumber) const = 0;
	};

}
}