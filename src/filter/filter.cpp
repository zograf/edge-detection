#include "filter.h"
#include "../bitmap/BitmapRawConverter.h"
#include <vector>
#include <iostream>
#include <string>

image_filter::Filter::Filter() {
    // char* output[4];
    // output[0] = (char*) "../../out/serialPrewitt.bmp";
    // output[1] = (char*) "../../out/parallelPrewitt.bmp";
    // output[2] = (char*) "../../out/serialEdge.bmp";
    // output[3] = (char*) "../../out/parallelEdge.bmp";
    // char* path = (char*) "../../res/color.bmp";

	// BitmapRawConverter inputFile(path);
	// BitmapRawConverter outputFileSerialPrewitt(path);
	// BitmapRawConverter outputFileParallelPrewitt(path);
	// BitmapRawConverter outputFileSerialEdge(path);
	// BitmapRawConverter outputFileParallelEdge(path);

	// int test;
	// 
	// picture_width = inputFile.getWidth();
	// picture_height = inputFile.getHeight();

	// int* outBufferSerialPrewitt = new int[picture_width * picture_height];
	// int* outBufferParallelPrewitt = new int[picture_width * picture_height];
    // int* outBufferSerialEdge = new int[picture_width * picture_height];
    // int* outBufferParallelEdge = new int[picture_width * picture_height];


    // memset(outBufferSerialPrewitt, 0x0, picture_width * picture_height * sizeof(int));
    // memset(outBufferParallelPrewitt, 0x0, picture_width * picture_height * sizeof(int));
	// memset(outBufferSerialEdge, 0x0, picture_width * picture_height * sizeof(int));
	// memset(outBufferParallelEdge, 0x0, picture_width * picture_height * sizeof(int));

	// inputFile.setBuffer(outBufferSerialPrewitt);
	// inputFile.pixelsToBitmap(output[0]);
}

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

int image_filter::calculate_p_o(int *in_matrix, int picture_width, 
        int x, int y, int distance) {
    int p = 0, o = 0;
    int offset = (distance - 1) / 2;
    // Surrounding pixels
    for (int ver = 0; ver < distance; ver++) {
        for (int hor = 0; hor < distance; hor++) {
            // if "1" exists
            if (in_matrix[(x - offset + ver) * picture_width + (y - offset + hor)] > THRESHOLD) p = 1;
            // if "0" exists
            if (in_matrix[(x - offset + ver) * picture_width + (y - offset + hor)] < THRESHOLD) o = 1;
        }
    }
    return (p ^ o) == 1 ? 0 : 255;
}

void image_filter::Filter::serial(int *in_matrix, int *out_matrix, dimension dim, 
        void (Filter::*serial_function)(int*, int*, struct dimension)) {
    (this->*serial_function)(in_matrix, out_matrix, dim);
}

void image_filter::Filter::serial_prewitt(int *in_matrix, int *out_matrix, struct dimension dim) {
    for (int i = dim.start_h; i < dim.end_h; i++) {
        for (int j = dim.start_w; j < dim.end_w; j++) {
            out_matrix[i * this->picture_width + j] = prewitt_convolve(in_matrix, this->filter_h, this->filter_v, 
                    this->filter_size, this->picture_width, i, j);
        }
    }
}

void image_filter::Filter::serial_edge_detection(int *in_matrix, int *out_matrix, struct dimension dim) {
    for (int i = dim.start_h; i < dim.end_h; i++) {
        for (int j = dim.start_w; j < dim.end_w; j++) {
            out_matrix[i * this->picture_width + j] = calculate_p_o(in_matrix, this->picture_width, 
                    i, j, this->distance);
        }
    }
}


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
