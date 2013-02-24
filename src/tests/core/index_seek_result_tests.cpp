#define BOOST_TEST_MODULE ImgDetectiveUnitTests
#include <boost/test/unit_test.hpp>
#include "core/IndexSeekResult.h"

using namespace ImgDetective::Core;

BOOST_AUTO_TEST_SUITE(index_seek_result_tests)

BOOST_AUTO_TEST_CASE(results_are_sorted_in_asc_order) {
	IndexSeekResult result(1);
	result.Insert(1, FeatureDistance(0.7));
	result.Insert(3, FeatureDistance(0.2));
	result.Insert(2, FeatureDistance(0.3));
	result.Insert(4, FeatureDistance(0.82));
	result.Insert(5, FeatureDistance(0.6));
	result.Insert(6, FeatureDistance(0.3));

	IndexSeekResult::entry_col_t entries = result.GetEntries();

	bool ascOrder = true;
	FeatureDistance prevDistance(0);
	FeatureDistance curDistance(0);

	IndexSeekResult::entry_col_t::const_iterator it = entries.cbegin();
	prevDistance = it->distance;
	++it;

	for (; it != entries.cend(); ++it) {
		curDistance = it->distance;

		if (curDistance < prevDistance) {
			ascOrder = false;
			break;
		}
	}

	BOOST_CHECK(ascOrder);
}

BOOST_AUTO_TEST_SUITE_END();