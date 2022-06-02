#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <string>
#include "../bitmap/BitmapRawConverter.h"
#include "tbb/task_group.h"
#include <tbb/tick_count.h>
#include <chrono>
#include <vector>

#pragma once

namespace image_filter {

    const int PREWITT_HORIZONTAL_3x3[] = {-1, 0, 1, 
                                        -1, 0, 1, 
                                        -1, 0, 1};

    const int PREWITT_VERTICAL_3x3[] = {-1, -1, -1, 
                                        0, 0, 0, 
                                        1, 1, 1};

    const int PREWITT_HORIZONTAL_5x5[] = { 9, 9, 9, 9, 9, 
                                            9, 5, 5, 5, 9, 
                                            -7, -3, 0, -3, -7, 
                                            -7, -3, -3, -3, -7,
                                            -7, -7, -7, -7, -7};

    const int PREWITT_VERTICAL_5x5[] = {9, 9, -7, -7, -7, 
                                        9, 5, -3, -3, -7, 
                                        9, 5, 0, -3, -7, 
                                        9, 5, -3, -3, -7, 
                                        9, 9, -7, -7, -7};

    const int THRESHOLD = 128;

    struct dimension {
        int start_w;
        int end_w;
        int start_h;
        int end_h;
    };

    enum filter_type {
        PREWITT_OPERATOR,
        EDGE_DETECTION
    };

    class Filter {
        private:
            int picture_width;
            int picture_height;

            int const* filter_h;
            int const* filter_v;
            int filter_size;

            int distance;
            int cutoff;

            void serial(int *in_matrix, int *out_matrix, 
                    dimension dim, void (Filter::*f)(int*, int*, dimension));
            void parallel(int *in_matrix, int *out_matrix, 
                    dimension dim, void (Filter::*f)(int*, int*, dimension));

            void serial_prewitt(int *in_matrix, int *out_matrix, dimension dim);
            void serial_edge_detection(int *in_matrix, int *out_matrix, dimension dim);
        public:
            Filter();
            ~Filter() { }

            void apply_serial(int *in_matrix, int* out_matrix, filter_type type);
            void apply_parallel(int *in_matrix, int* out_matrix, filter_type type);

            void set_filter(int filter_size); 
            void set_width(int width) { this->picture_width = width; }
            void set_height(int height) { this->picture_height = height; }
            void set_distance(int distance) {this->distance = distance * 2 + 1; }
            void set_cutoff(int cutoff) { this->cutoff = cutoff; }
    };

    int prewitt_convolve(int *in_matrix, const int* filter_h, 
            const int* filter_v, int filter_size, int picture_width, int x, int y);
    int calculate_p_o(int *in_matrix, int picutre_width, int x, int y, int distance);
}
