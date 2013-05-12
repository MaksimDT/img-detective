#pragma once

#include "core/Feature.h"
#include "utils/MathUtils.h"

#define COLOR_HISTOGRAM_BIN_COUNT 256
#define NUMBER_OF_BIN_VALUES 65536
#define MAX_BIN_VALUE (NUMBER_OF_BIN_VALUES - 1)
#define COLOR_HISTOGRAM_FTYPE_ID 1 /*Feature type ID of color histogram feature*/

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    CONCRETE class ColorHistogramFeat : NONCOPYABLE, public Core::Feature {
    public:
        #pragma region NESTED

        CONCRETE NESTED class ChannelHistogram : NONCOPYABLE {
        public:
            typedef unsigned short bin_value_t;
            typedef std::vector<bin_value_t> bins_vector_t;

            CTOR ChannelHistogram(bins_vector_t* bins);
            ~ChannelHistogram();
            static ChannelHistogram* Deserialize(const Core::blob_t& buffer, REF size_t& offset);

            void SerializeToBuffer(REF Core::blob_t& buffer, REF size_t& offset) const;
            double ComputeDistanceTo(const REF ChannelHistogram& other) const;
        private:
            /*index of vector corresponds to tonal value, value of element at this index
            corresponds to the relative number of pixels that have this tonal value*/
            bins_vector_t* bins;
        };

        #pragma endregion

        CTOR ColorHistogramFeat(ChannelHistogram* r, ChannelHistogram* g, ChannelHistogram* b);
        ~ColorHistogramFeat();
        static ColorHistogramFeat* Deserialize(const Core::blob_t& blob);

        virtual Core::blob_p_t Serialize() const;
    protected:
        virtual Core::FeatureDistance ComputeDistanceInternal(const REF Core::IFeature& feature) const;
    private:
        ChannelHistogram* r;
        ChannelHistogram* g;
        ChannelHistogram* b;
	};

}
}
}