#include "matching.h"

matching::matching(std::string left_path, std::string right_path)
{
    left_img = cv::imread(left_path);
    right_img = cv::imread(right_path);
}

void matching::set_params(int size)
{
    window_size = size;
}