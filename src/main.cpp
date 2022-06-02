#include "filter/filter.h"
#include "../test/test.h"
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

int main(int argc, char * argv[]) {
    // Output files

    // image_filter::Filter f;
    // f.set_cutoff(500);
    // f.set_distance(1);
    // f.set_filter(3);
    // f.set_width(picture_width);
    // f.set_height(picture_height);

    // auto start = std::chrono::high_resolution_clock::now();
    // f.apply_serial(outputFileSerialPrewitt.getBuffer(), outBufferSerialPrewitt, image_filter::PREWITT_OPERATOR);
    // auto end = std::chrono::high_resolution_clock::now();
    // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // cout << elapsed << " milliseconds" << endl;
	// outputFileSerialPrewitt.setBuffer(outBufferSerialPrewitt);
	// outputFileSerialPrewitt.pixelsToBitmap(output[0]);

    // start = std::chrono::high_resolution_clock::now();
    // f.apply_parallel(outputFileParallelPrewitt.getBuffer(), outBufferParallelPrewitt, image_filter::PREWITT_OPERATOR);
    // end = std::chrono::high_resolution_clock::now();
    // elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // cout << elapsed << " milliseconds" << endl;
	// outputFileParallelPrewitt.setBuffer(outBufferParallelPrewitt);
	// outputFileParallelPrewitt.pixelsToBitmap(output[1]);

    // start = std::chrono::high_resolution_clock::now();
    // f.apply_serial(outputFileSerialEdge.getBuffer(), outBufferSerialEdge, image_filter::EDGE_DETECTION);
    // end = std::chrono::high_resolution_clock::now();
    // elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // cout << elapsed << " milliseconds" << endl;
	// outputFileSerialEdge.setBuffer(outBufferSerialEdge);
	// outputFileSerialEdge.pixelsToBitmap(output[2]);

    // start = std::chrono::high_resolution_clock::now();
    // f.apply_parallel(outputFileParallelEdge.getBuffer(), outBufferParallelEdge, image_filter::EDGE_DETECTION);
    // end = std::chrono::high_resolution_clock::now();
    // elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // cout << elapsed << " milliseconds" << endl;
	// outputFileParallelEdge.setBuffer(outBufferParallelEdge);
	// outputFileParallelEdge.pixelsToBitmap(output[3]);
    // /*
	// // serial version Prewitt
	// run_test_nr(1, &outputFileSerialPrewitt, output[0], outBufferSerialPrewitt);

	// // parallel version Prewitt
	// run_test_nr(2, &outputFileParallelPrewitt, output[1], outBufferParallelPrewitt);

	// // serial version special
	// run_test_nr(3, &outputFileSerialEdge, output[2], outBufferSerialEdge);

	// // parallel version special
	// run_test_nr(4, &outputFileParallelEdge, output[3], outBufferParallelEdge);

	// // verification
    // */
	// cout << "Verification: ";
	// test = memcmp(outBufferSerialPrewitt, outBufferParallelPrewitt, picture_width * picture_height * sizeof(int));

	// if(test != 0)
	// 	cout << "Prewitt FAIL!" << endl;
    // else
	// 	cout << "Prewitt PASS." << endl;

	// test = memcmp(outBufferSerialEdge, outBufferParallelEdge, picture_width * picture_height * sizeof(int));

	// if(test != 0)
	// 	cout << "Edge detection FAIL!" << endl;
	// else
	// 	cout << "Edge detection PASS." << endl;

	// // clean up
	// delete[] outBufferSerialPrewitt;
	// delete[] outBufferParallelPrewitt;
	// delete[] outBufferSerialEdge;
	// delete[] outBufferParallelEdge;

    filter_test::prewitt_convolve_test();

	return 0;
} 
