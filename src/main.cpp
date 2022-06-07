#include "filter/filter.h"
#include "../test/test.h"
#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
    // Unit tests
    // filter_test::prewitt_convolve_test();
    // filter_test::calculate_p_o_test();

    // Normal run
    //image_filter::Filter filter;
    //filter.run(true);

    filter_test::stress_test();

	return 0;
} 
