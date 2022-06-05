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

            void serial(int*, int*, dimension, void (Filter::*)(int*, int*, dimension));
            void parallel(int*, int*, dimension, void (Filter::*)(int*, int*, dimension));

            void serial_prewitt(int*, int*, dimension);
            void serial_edge_detection(int*, int*, dimension);
        public:
            Filter();
            ~Filter() { }

            void apply_serial(int*, int*, filter_type);
            void apply_parallel(int*, int*, filter_type);
            void run();

            void set_filter(int); 
            void set_width(int);
            void set_height(int);
            void set_distance(int);
            void set_cutoff(int);
    };

    int prewitt_convolve(int*, const int*, const int*, int, int, int, int);
    int calculate_p_o(int*, int, int, int, int);
}
