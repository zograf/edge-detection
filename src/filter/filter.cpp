#include "filter.h"
#include <vector>
#include <iostream>
#include <string>

/**
* @brief Edge detection filter constructor
*/

image_filter::Filter::Filter() {
}

/**
* @brief Function for running test.
*
* @param is_defaults Flag that enables setting default cutoff, distance and filter values.
* Leave "false" for normal running. Put "true" if cutoff, distance and filter have already
* been set
*/
void image_filter::Filter::run(bool is_defaults) {
    char* output[4];
    output[0] = (char*) "../../out/serialPrewitt.bmp";
    output[1] = (char*) "../../out/serialEdge.bmp";
    output[2] = (char*) "../../out/parallelPrewitt.bmp";
    output[3] = (char*) "../../out/parallelEdge.bmp";
    char* path = (char*) "../../res/sackboy.bmp";

	BitmapRawConverter inputFile(path);
	BitmapRawConverter outputFileSerialPrewitt(path);
	BitmapRawConverter outputFileParallelPrewitt(path);
	BitmapRawConverter outputFileSerialEdge(path);
	BitmapRawConverter outputFileParallelEdge(path);

    int p_width = inputFile.getWidth();
    int p_height = inputFile.getHeight();

    this->set_width(p_width);
    this->set_height(p_height);

    if (is_defaults) {
        this->set_cutoff(500);
        this->set_distance(1);
        this->set_filter(3);
    }

	int* outBufferSerialPrewitt = new int[p_width * p_height];
	int* outBufferParallelPrewitt = new int[p_width * p_height];
    int* outBufferSerialEdge = new int[p_width * p_height];
    int* outBufferParallelEdge = new int[p_width * p_height];

    std::memset(outBufferSerialPrewitt, 0x0, p_width * p_height * sizeof(int));
    std::memset(outBufferParallelPrewitt, 0x0, p_width * p_height * sizeof(int));
    std::memset(outBufferSerialEdge, 0x0, p_width * p_height * sizeof(int));
    std::memset(outBufferParallelEdge, 0x0, p_width * p_height * sizeof(int));

    auto start = std::chrono::high_resolution_clock::now();
    this->apply_serial(outputFileSerialPrewitt.getBuffer(), outBufferSerialPrewitt, image_filter::PREWITT_OPERATOR);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    outputFileSerialPrewitt.setBuffer(outBufferSerialPrewitt);
    outputFileSerialPrewitt.pixelsToBitmap(output[0]);
    std::cout << path << ",prewitt," << "serial,"<< this->cutoff << ",," << this->filter_size << "," << elapsed << std::endl;

    start = std::chrono::high_resolution_clock::now();
    this->apply_serial(outputFileSerialEdge.getBuffer(), outBufferSerialEdge, image_filter::EDGE_DETECTION);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    outputFileSerialEdge.setBuffer(outBufferSerialEdge);
    outputFileSerialEdge.pixelsToBitmap(output[1]);
    std::cout << path << ",edge," << "serial,"<< this->cutoff << "," << ((this->distance - 1) / 2) << ",," << elapsed << std::endl;

    start = std::chrono::high_resolution_clock::now();
    this->apply_parallel(outputFileParallelPrewitt.getBuffer(), outBufferParallelPrewitt, image_filter::PREWITT_OPERATOR);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    outputFileParallelPrewitt.setBuffer(outBufferParallelPrewitt);
    outputFileParallelPrewitt.pixelsToBitmap(output[2]);
    std::cout << path << ",prewitt," << "parallel,"<< this->cutoff << ",," << this->filter_size << "," << elapsed << std::endl;

    start = std::chrono::high_resolution_clock::now();
    this->apply_parallel(outputFileParallelEdge.getBuffer(), outBufferParallelEdge, image_filter::EDGE_DETECTION);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    outputFileParallelEdge.setBuffer(outBufferParallelEdge);
    outputFileParallelEdge.pixelsToBitmap(output[3]);
    std::cout << path << ",edge," << "parallel,"<< this->cutoff << "," << ((this->distance - 1) / 2) << ",," << elapsed << std::endl;

	delete[] outBufferSerialPrewitt;
	delete[] outBufferParallelPrewitt;
	delete[] outBufferSerialEdge;
	delete[] outBufferParallelEdge;
}

/**
* @brief Prewitt convolve function
*
* @param in_matrix Input image buffer 
* @param filter_h Pointer to the horizontal prewitt operator
* @param filter_v Pointer to the vertical prewitt operator
* @param filter_size Prewitt operator size. 3 for 3x3, 5 for 5x5
* @param picture_width Image width
* @param x Pixel width coordinate
* @param y Pixel height coordinate
*/

int image_filter::prewitt_convolve(int *in_matrix, const int* filter_h, 
        const int* filter_v, int filter_size, int picture_width, int x, int y) {
    int sum_h = 0, sum_v = 0;
    int offset = (filter_size - 1) / 2;
    
    for (int i = 0; i < filter_size; i++) {
        for (int j = 0; j < filter_size; j++) {
            sum_h += filter_h[i * filter_size + j] * in_matrix[(x - offset + i) * picture_width + (y - offset + j)];
            sum_v += filter_v[i * filter_size + j] * in_matrix[(x - offset + i) * picture_width + (y - offset + j)];
        }
    }
    
    int total = abs(sum_h) + abs(sum_v);
    return (total > THRESHOLD) ? 255 : 0;
}


/**
* @brief Function that calculates p and o values for edge detection algorithm
*
* @param in_matrix Input image buffer 
* @param picture_width Image width
* @param x Pixel width coordinate
* @param y Pixel height coordinate
* @param distance Distance from the target pixel
*
* @return Pixel value
*/

int image_filter::calculate_p_o(int *in_matrix, int picture_width, 
        int x, int y, int distance) {
    int p = 0, o = 1;
    int offset = (distance - 1) / 2;
    // Surrounding pixels
    for (int ver = 0; ver < distance; ver++) {
        for (int hor = 0; hor < distance; hor++) {
            // if "1" exists
            if (in_matrix[(x - offset + ver) * picture_width + (y - offset + hor)] > THRESHOLD) p = 1;
            // if "0" exists
            if (in_matrix[(x - offset + ver) * picture_width + (y - offset + hor)] < THRESHOLD) o = 0;
        }
    }
    return (p ^ o) == 1 ? 255 : 0;
}

/**
* @brief Function that calls appropriate serial version of an algorithm
*
* @param in_matrix Input image buffer 
* @param out_matrix Output image buffer 
* @param dimension Image boundary structure
* @param serial_function Pointer to a serial function that should be called
* @see serial_prewitt()
* @see serial_edge_detection()
*/

void image_filter::Filter::serial(int *in_matrix, int *out_matrix, dimension dim, 
        void (Filter::*serial_function)(int*, int*, struct dimension)) {
    (this->*serial_function)(in_matrix, out_matrix, dim);
}

/**
* @brief Function that applies a prewitt filter on an image
*
* @param in_matrix Input image buffer 
* @param out_matrix Output image buffer 
* @param dimension Image boundary structure
*/

void image_filter::Filter::serial_prewitt(int *in_matrix, int *out_matrix, struct dimension dim) {
    for (int i = dim.start_h; i < dim.end_h; i++) {
        for (int j = dim.start_w; j < dim.end_w; j++) {
            out_matrix[i * this->picture_width + j] = prewitt_convolve(in_matrix, this->filter_h, this->filter_v, 
                    this->filter_size, this->picture_width, i, j);
        }
    }
}

/**
* @brief Function that applies edge detection algorithm on an image
*
* @param in_matrix Input image buffer 
* @param out_matrix Output image buffer 
* @param dimension Image boundary structure
*/

void image_filter::Filter::serial_edge_detection(int *in_matrix, int *out_matrix, struct dimension dim) {
    for (int i = dim.start_h; i < dim.end_h; i++) {
        for (int j = dim.start_w; j < dim.end_w; j++) {
            out_matrix[i * this->picture_width + j] = calculate_p_o(in_matrix, this->picture_width, 
                    i, j, this->distance);
        }
    }
}

/**
* @brief Filter setter
*
* @param filter_size Size of a prewitt operator
*/

void image_filter::Filter::set_filter(int filter_size) {
    if (filter_size == 3) {
        this->filter_v = PREWITT_VERTICAL_3x3;
        this->filter_h = PREWITT_HORIZONTAL_3x3;
        this->filter_size = filter_size;
    } else if (filter_size == 5) {
        this->filter_v = PREWITT_VERTICAL_5x5;
        this->filter_h = PREWITT_HORIZONTAL_5x5;
        this->filter_size = filter_size;
    } else {
        throw std::invalid_argument("Filter size must be in {3, 5}.");
    }
}

/**
* @brief Function that applies a serial algorithm on an image 
*
* @param in_matrix Input image buffer 
* @param out_matrix Output image buffer 
* @param type Filter type
*/

void image_filter::Filter::apply_serial(int *in_matrix, int *out_matrix, filter_type type) {
    // Function pointer
    void (Filter::*p)(int*, int*, dimension);
    dimension dim;
    int offset;

    if (type == PREWITT_OPERATOR) {
        offset = (this->filter_size - 1) / 2;
        p = &Filter::serial_prewitt;
    } else {
        offset = (this->distance - 1) / 2;
        p = &Filter::serial_edge_detection;
    }

    dim.start_h = offset;
    dim.start_w = offset;
    dim.end_h = this->picture_height - offset;
    dim.end_w = this->picture_width - offset;
    serial(in_matrix, out_matrix, dim, p);
}

/**
* @brief Function that applies a parallel algorithm on an image 
*
* @param in_matrix Input image buffer 
* @param out_matrix Output image buffer 
* @param type Filter type
*/

void image_filter::Filter::apply_parallel(int *in_matrix, int *out_matrix, filter_type type) {
    // Function pointer
    void (Filter::*p)(int*, int*, dimension);
    dimension dim;
    int offset;

    if (type == PREWITT_OPERATOR) {
        offset = (this->filter_size - 1) / 2;
        p = &Filter::serial_prewitt;
    } else {
        offset = this->distance;
        p = &Filter::serial_edge_detection;
    }

    dim.start_h = offset;
    dim.start_w = offset;
    dim.end_h = this->picture_height - offset;
    dim.end_w = this->picture_width - offset;
    parallel(in_matrix, out_matrix, dim, p);
}

/**
* @brief Function that calls appropriate parallel version of an algorithm
*
* @param in_matrix Input image buffer 
* @param out_matrix Output image buffer 
* @param dimension Image boundary structure
* @param serial_function Pointer to a serial function that should be called
*/

void image_filter::Filter::parallel(int *in_matrix, int *out_matrix, dimension dim, 
        void (Filter::*serial_function)(int*, int*, struct dimension)) {

    // Serial check
    if (abs(dim.end_w - dim.start_w) <= this->cutoff || 
            abs(dim.end_h - dim.start_h) <= this->cutoff)
        (this->*serial_function)(in_matrix, out_matrix, dim);

    else {
        tbb::task_group tg;
        // 1
        tg.run([=]() {
                dimension d;
                d.start_w = dim.start_w;
                d.end_w = dim.start_w + (dim.end_w - dim.start_w) / 2;
                d.start_h = dim.start_h;
                d.end_h = dim.start_h + (dim.end_h - dim.start_h) / 2;
                parallel(in_matrix, out_matrix, d, serial_function);
        });
        // 2
        tg.run([=]() {
                dimension d;
                d.start_w = dim.start_w;
                d.end_w = dim.start_w + (dim.end_w - dim.start_w) / 2;
                d.start_h = dim.start_h + (dim.end_h - dim.start_h) / 2;
                d.end_h = dim.end_h;
                parallel(in_matrix, out_matrix, d, serial_function);
        });
        // 3
        tg.run([=]() {
                dimension d;
                d.start_w = dim.start_w + (dim.end_w - dim.start_w) / 2;
                d.end_w = dim.end_w;
                d.start_h = dim.start_h;
                d.end_h = dim.start_h + (dim.end_h - dim.start_h) / 2;
                parallel(in_matrix, out_matrix, d, serial_function);
        });
        // 4
        tg.run([=]() {
                dimension d;
                d.start_w = dim.start_w + (dim.end_w - dim.start_w) / 2;
                d.end_w = dim.end_w;
                d.start_h = dim.start_h + (dim.end_h - dim.start_h) / 2;
                d.end_h = dim.end_h;
                parallel(in_matrix, out_matrix, d, serial_function);
        });
        tg.wait();
    }
}

/**
* @brief Width setter
*
* @param width Image width
*/

void image_filter::Filter::set_width(int width) { 
    this->picture_width = width;
}

/**
* @brief Height setter
*
* @param height Image height
*/

void image_filter::Filter::set_height(int height) { 
    this->picture_height = height;
}

/**
* @brief Distance setter
*
* @param distance Pixel distance from the target pixel
*/

void image_filter::Filter::set_distance(int distance) {
    this->distance = distance * 2 + 1;
}


/**
* @brief Cutoff setter
*
* @param cutoff Parallel algorithm cutoff
*/

void image_filter::Filter::set_cutoff(int cutoff) { 
    this->cutoff = cutoff;
}

