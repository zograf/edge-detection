#include "test.h"

filter_test::Test::Test() {
}

void filter_test::prewitt_convolve_test() {
    // Filters
    int const* h_3x3 = image_filter::PREWITT_HORIZONTAL_3x3;
    int const* v_3x3 = image_filter::PREWITT_VERTICAL_3x3;
    int const* h_5x5 = image_filter::PREWITT_HORIZONTAL_5x5;
    int const* v_5x5 = image_filter::PREWITT_VERTICAL_5x5;

    int num_test = 5;

    int in_first[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int expected_first = 50;
    int in_second[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int expected_second = 0;
    int in_third[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int expected_third = 0;
    int in_fourth[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int expected_fourth = 0;
    int in_fifth[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int expected_fifth = 0;

    int result_first = image_filter::prewitt_convolve(in_first, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_second = image_filter::prewitt_convolve(in_first, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_third = image_filter::prewitt_convolve(in_first, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_fourth = image_filter::prewitt_convolve(in_first, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_fifth = image_filter::prewitt_convolve(in_first, h_3x3, v_3x3, 3, 3, 1, 1);

    print_test(result_first, expected_first, 1, num_test);
    print_test(result_second, expected_second, 2, num_test);
    print_test(result_third, expected_third, 3, num_test);
    print_test(result_fourth, expected_fourth, 4, num_test);
    print_test(result_fifth, expected_fifth, 5, num_test);
}

void filter_test::print_test(int first, int second, int number, int total) {
    std::string s = "[" + std::to_string(number)  + "/" + std::to_string(total) + "] ";
    if (first == second) s += "OK";
    else s += "FAIL";
    std::cout << s << std::endl;
}


void filter_test::calculate_p_o_test() {

}

