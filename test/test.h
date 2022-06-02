#include "../src/filter/filter.h"
#include <cassert>
#include <iostream>
#include <string>

#pragma once

namespace filter_test {
    class Test {
        image_filter::Filter filter;
        private:
        public:
            Test();
            ~Test() { }
    };

    void calculate_p_o_test();
    void prewitt_convolve_test();
    void print_test(int, int, int, int);
}
