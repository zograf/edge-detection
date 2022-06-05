#include "filter/filter.h"
#include "../test/test.h"
#include <iostream>
/**
* @brief Function for running test.
*
* @param testNr test identification, 1: for serial version, 2: for parallel version
* @param ioFile input/output file, firstly it's holding buffer from input image and than to hold filtered data
* @param outFileName output file name
* @param outBuffer buffer of output image
* @param width image width
* @param height image height
*/

using namespace std;

int main(int argc, char * argv[]) {
    // Unit tests
    filter_test::prewitt_convolve_test();
    filter_test::calculate_p_o_test();

    // Run
    image_filter::Filter filter;
    filter.run();

	return 0;
} 
