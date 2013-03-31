#pragma once

#include "core/Feature.h"
#include "utils/MathUtils.h"

//TODO: get rid of it

#define COLOR_HISTOGRAM_BIN_COUNT ImgDetective::Utils::Math::GetNumberOfStates(sizeof(ImgDetective::Core::pixel_tone_t))
#define COLOR_HISTOGRAM_FTYPE_ID 1 /*Feature type ID of color histogram feature*/

namespace ImgDetective {
namespace Features {

	CONCRETE class ColorHistogramFeat : public Core::Feature {
	public:
        #pragma region NESTED

        CONCRETE NESTED class ChannelHistogram {
        public:
            typedef Core::byte_t bin_value_t;

            static size_t GetRequiredBufferSize();

            CTOR ChannelHistogram(const REF std::vector<bin_value_t>& bins);

            void SerializeToBuffer(const REF Core::blob_t& buffer, REF Core::blob_t::iterator& bufferIt) const;
            double ComputeDistanceTo(const REF ChannelHistogram& other) const;
        private:
            /*index of vector corresponds to tonal value, value of element at this index
            corresponds to the relative number of pixels that have this tonal value*/
            std::vector<Core::byte_t> bins;
        };

        #pragma endregion

        CTOR ColorHistogramFeat(const REF ChannelHistogram& r, const REF ChannelHistogram& g, const REF ChannelHistogram& b);

        virtual Core::blob_p_t Serialize() const;
    protected:
        virtual Core::FeatureDistance ComputeDistanceInternal(const REF Core::IFeature& feature) const;
    private:
        ChannelHistogram r;
        ChannelHistogram g;
        ChannelHistogram b;
	};

}
}