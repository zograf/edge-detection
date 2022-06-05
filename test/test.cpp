#include "test.h"

filter_test::Test::Test() {
}

void filter_test::prewitt_convolve_test() {
    // Filters
    int const* h_3x3 = image_filter::PREWITT_HORIZONTAL_3x3;
    int const* v_3x3 = image_filter::PREWITT_VERTICAL_3x3;
    int const* h_5x5 = image_filter::PREWITT_HORIZONTAL_5x5;
    int const* v_5x5 = image_filter::PREWITT_VERTICAL_5x5;

    int num_test = 10;

    int in_first[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expected_first = 0;
    int in_second[] = {255, 255, 255, -1, -1, 5, 7, 2, -3};
    int expected_second = 255;
    int in_third[] = {10, 11, -15, -5, 0, -7, -9, 9, 21};
    int expected_third = 0;
    int in_fourth[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    int expected_fourth = 0;
    int in_fifth[] = {125, 212, 198, 111, 100, 54, 55, 208, 201};
    int expected_fifth = 255;

    int result_first = image_filter::prewitt_convolve(in_first, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_second = image_filter::prewitt_convolve(in_second, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_third = image_filter::prewitt_convolve(in_third, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_fourth = image_filter::prewitt_convolve(in_fourth, h_3x3, v_3x3, 3, 3, 1, 1);
    int result_fifth = image_filter::prewitt_convolve(in_fifth, h_3x3, v_3x3, 3, 3, 1, 1);

    std::cout << "\n[UNIT TEST] prewitt_convolve - 3x3" << std::endl;
    print_test(result_first, expected_first, 1, num_test);
    print_test(result_second, expected_second, 2, num_test);
    print_test(result_third, expected_third, 3, num_test);
    print_test(result_fourth, expected_fourth, 4, num_test);
    print_test(result_fifth, expected_fifth, 5, num_test);

    int in_first_[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    expected_first = 255;
    int in_second_[] = {255, 255, 255, 255, 255, 6, 7, 8, 9, 10, 11, 12, 13, 14, 255, 255, 255, 255, 255, 255, 21, 22, 23, 24, 25};
    expected_second = 255;
    int in_third_[] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16, -17, -18, -19, -20, -21, -22, -23, -24, -25};
    expected_third = 255;
    int in_fourth_[] = {1, 2, 3, -4, 5, -6, 7, 8, -9, -10, -11, -12, -13, -14, -15, -16, -17, -18, 19, 20, 21, -22, 23, -24, 25};
    expected_fourth = 255;
    int in_fifth_[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    expected_fifth = 0;

    result_first = image_filter::prewitt_convolve(in_first_, h_5x5, v_5x5, 5, 5, 2, 2);
    result_second = image_filter::prewitt_convolve(in_second_, h_5x5, v_5x5, 5, 5, 2, 2);
    result_third = image_filter::prewitt_convolve(in_third_, h_5x5, v_5x5, 5, 5, 2, 2);
    result_fourth = image_filter::prewitt_convolve(in_fourth_, h_5x5, v_5x5, 5, 5, 2, 2);
    result_fifth = image_filter::prewitt_convolve(in_fifth_, h_5x5, v_5x5, 5, 5, 2, 2);

    std::cout << "\n[UNIT TEST] prewitt_convolve - 5x5" << std::endl;
    print_test(result_first, expected_first, 6, num_test);
    print_test(result_second, expected_second, 7, num_test);
    print_test(result_third, expected_third, 8, num_test);
    print_test(result_fourth, expected_fourth, 9, num_test);
    print_test(result_fifth, expected_fifth, 10, num_test);
}

void filter_test::print_test(int first, int second, int number, int total) {
    std::string s = "[" + std::to_string(number)  + "/" + std::to_string(total) + "] ";
    if (first == second) s += "OK";
    else s += "FAIL";
    std::cout << s << std::endl;
}


void filter_test::calculate_p_o_test() {

    int num_test = 6;

    int in_first[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expected_first = 0;
    int in_second[] = {255, 255, 255, 255, 255, 255, 255, 255, 255};
    int expected_second = 0;
    int in_third[] = {255, 255, 255, 1, 1, 1, 1, 1, 1};
    int expected_third = 255;

    int result_first = image_filter::calculate_p_o(in_first, 3, 1, 1, 3);
    int result_second = image_filter::calculate_p_o(in_second, 3, 1, 1, 3);
    int result_third = image_filter::calculate_p_o(in_third, 3, 1, 1, 3);

    std::cout << "\n[UNIT TEST] calculate_p_o - distance:1" << std::endl;
    print_test(result_first, expected_first, 1, num_test);
    print_test(result_second, expected_second, 2, num_test);
    print_test(result_third, expected_third, 3, num_test);

    int in_first_[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    int expected_first_ = 0;
    int in_second_[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    int expected_second_ = 0;
    int in_third_[] = {255, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    int expected_third_ = 255;

    int result_first_ = image_filter::calculate_p_o(in_first_, 3, 1, 1, 3);
    int result_second_ = image_filter::calculate_p_o(in_second_, 3, 1, 1, 3);
    int result_third_ = image_filter::calculate_p_o(in_third_, 3, 1, 1, 3);

    std::cout << "\n[UNIT TEST] calculate_p_o - distance:2" << std::endl;
    print_test(result_first_, expected_first_, 4, num_test);
    print_test(result_second_, expected_second_, 5, num_test);
    print_test(result_third_, expected_third_, 6, num_test);
}

