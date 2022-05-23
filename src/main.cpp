#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <string>
#include "BitmapRawConverter.h"
#include "tbb/task_group.h"
#include <tbb/tick_count.h>
#include <chrono>

#define __ARG_NUM__				6
#define FILTER_SIZE				3
#define THRESHOLD				128
#define PREWITT_CUTOFF          1500

using namespace std;
using namespace tbb;

// Prewitt operators
const int horizontal_filter[FILTER_SIZE * FILTER_SIZE] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
const int vertical_filter[FILTER_SIZE * FILTER_SIZE] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

int picture_width, picture_height;

void multiply(int* in_matrix, int* out_matrix, int x, int y) { 
    int sum_h = 0, sum_v = 0;
    for (int i = 0; i < FILTER_SIZE; i++) {
        for (int j = 0; j < FILTER_SIZE; j++) {
            sum_h += horizontal_filter[i * FILTER_SIZE + j] * in_matrix[(x + i) * picture_width + (y + j)];
            sum_v += vertical_filter[i * FILTER_SIZE + j] * in_matrix[(x + i) * picture_width + (y + j)];
        }
    }

    int total = abs(sum_h) + abs(sum_v);
    out_matrix[x * picture_width + y] = (total > THRESHOLD) ? 255 : 0;
}

void serial_prewitt(int *inBuffer, int *outBuffer, int starting_width, int ending_width, 
        int starting_height, int ending_height) {

    for (int i = starting_height; i < ending_height; i++) {
        for (int j = starting_width; j < ending_width; j++) {
            multiply(inBuffer, outBuffer, i, j);
        }
    }
}


/**
* @brief Serial version of edge detection algorithm implementation using Prewitt operator
*
* @param inBuffer buffer of input image
* @param outBuffer buffer of output image
* @param width image width
* @param height image height
*/
void filter_serial_prewitt(int *inBuffer, int *outBuffer) {
    serial_prewitt(inBuffer, outBuffer, 1, picture_width - 1, 1, picture_height - 1);
}


void parallel_prewitt(int *inBuffer, int *outBuffer, int starting_width, int ending_width, 
        int starting_height, int ending_height) {

    if (abs(ending_width - starting_width) <= PREWITT_CUTOFF || abs(ending_height - starting_height) <= PREWITT_CUTOFF) { 
        serial_prewitt(inBuffer, outBuffer, starting_width, ending_width, starting_height, ending_height);
        return;
    }

    task_group tg;
    // 1
    tg.run([&]() {
            parallel_prewitt(inBuffer, outBuffer, starting_width, ending_width / 2, starting_height, ending_height / 2);
    });
    // 2
    tg.run([&]() {
            parallel_prewitt(inBuffer, outBuffer, starting_width, ending_width / 2, ending_height / 2, ending_height);
    });
    // 3
    tg.run([&]() {
            parallel_prewitt(inBuffer, outBuffer, ending_width / 2, ending_width, starting_height, ending_height / 2);
    });
    // 4
    tg.run([&]() {
            parallel_prewitt(inBuffer, outBuffer, ending_width / 2, ending_width, ending_height / 2, ending_height);
    });
    tg.wait();
}

/**
 * @brief Parallel version of edge detection algorithm implementation using Prewitt operator
 * 
 * @param inBuffer buffer of input image
 * @param outBuffer buffer of output image
 * @param width image width
 * @param height image height
 */
void filter_parallel_prewitt(int *inBuffer, int *outBuffer) {
    parallel_prewitt(inBuffer, outBuffer, 1, picture_width - 1, 1, picture_height - 1);
}

/**
* @brief Serial version of edge detection algorithm
*
* @param inBuffer buffer of input image
* @param outBuffer buffer of output image
* @param width image width
* @param height image height
*/
void filter_serial_edge_detection(int *inBuffer, int *outBuffer, int width, int height) {
}

/**
* @brief Parallel version of edge detection algorithm
* 
* @param inBuffer buffer of input image
* @param outBuffer buffer of output image
* @param width image width
* @param height image height
*/
void filter_parallel_edge_detection(int *inBuffer, int *outBuffer, int width, int height) {
}

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


void run_test_nr(int testNr, BitmapRawConverter* ioFile, 
        char* outFileName, int* outBuffer, unsigned int width, unsigned int height) {

    auto start = std::chrono::high_resolution_clock::now();
	switch (testNr) {
		case 1:
			cout << "Running serial version of edge detection using Prewitt operator" << endl;
			filter_serial_prewitt(ioFile->getBuffer(), outBuffer);
			break;
		case 2:
			cout << "Running parallel version of edge detection using Prewitt operator" << endl;
			filter_parallel_prewitt(ioFile->getBuffer(), outBuffer);
			break;
		case 3:
			cout << "Running serial version of edge detection" << endl;
			filter_serial_edge_detection(ioFile->getBuffer(), outBuffer, width, height);
			break;
		case 4:
			cout << "Running parallel version of edge detection" << endl;
			filter_parallel_edge_detection(ioFile->getBuffer(), outBuffer, width, height);
			break;
		default:
			cout << "ERROR: invalid test case, must be 1, 2, 3 or 4!";
			break;
	}
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << elapsed << " milliseconds" << endl;

	ioFile->setBuffer(outBuffer);
	ioFile->pixelsToBitmap(outFileName);
}

int main(int argc, char * argv[]) {
    // Output files
    char* output[4];
    output[0] = (char*) "../../out/serialPrewitt.bmp";
    output[1] = (char*) "../../out/parallelPrewitt.bmp";
    output[2] = (char*) "../../out/serialEdge.bmp";
    output[3] = (char*) "../../out/parallelEdge.bmp";
    char* path = (char*) "../../res/color.bmp";

	BitmapRawConverter inputFile(path);
	BitmapRawConverter outputFileSerialPrewitt(path);
	BitmapRawConverter outputFileParallelPrewitt(path);
	BitmapRawConverter outputFileSerialEdge(path);
	BitmapRawConverter outputFileParallelEdge(path);

	int test;
	
	picture_width = inputFile.getWidth();
	picture_height = inputFile.getHeight();

	int* outBufferSerialPrewitt = new int[picture_width * picture_height];
	int* outBufferParallelPrewitt = new int[picture_width * picture_height];

	memset(outBufferSerialPrewitt, 0x0, picture_width * picture_height * sizeof(int));
	memset(outBufferParallelPrewitt, 0x0, picture_width * picture_height * sizeof(int));

	int* outBufferSerialEdge = new int[picture_width * picture_height];
	int* outBufferParallelEdge = new int[picture_width * picture_height];

	memset(outBufferSerialEdge, 0x0, picture_width * picture_height * sizeof(int));
	memset(outBufferParallelEdge, 0x0, picture_width * picture_height * sizeof(int));

	inputFile.setBuffer(outBufferSerialPrewitt);
	inputFile.pixelsToBitmap(output[0]);
	// serial version Prewitt
	run_test_nr(1, &outputFileSerialPrewitt, output[0], outBufferSerialPrewitt, picture_width, picture_height);

	// parallel version Prewitt
	run_test_nr(2, &outputFileParallelPrewitt, output[1], outBufferParallelPrewitt, picture_width, picture_height);

	// serial version special
	run_test_nr(3, &outputFileSerialEdge, output[2], outBufferSerialEdge, picture_width, picture_height);

	// parallel version special
	run_test_nr(4, &outputFileParallelEdge, output[3], outBufferParallelEdge, picture_width, picture_height);

	// verification
    /*
	cout << "Verification: ";
	test = memcmp(outBufferSerialPrewitt, outBufferParallelPrewitt, width * height * sizeof(int));

	if(test != 0)
		cout << "Prewitt FAIL!" << endl;
    else
		cout << "Prewitt PASS." << endl;

	test = memcmp(outBufferSerialEdge, outBufferParallelEdge, width * height * sizeof(int));

	if(test != 0)
		cout << "Edge detection FAIL!" << endl;
	else
		cout << "Edge detection PASS." << endl;

    */
	// clean up
	delete[] outBufferSerialPrewitt;
	delete[] outBufferParallelPrewitt;

	delete[] outBufferSerialEdge;
	delete[] outBufferParallelEdge;

	return 0;
} 
