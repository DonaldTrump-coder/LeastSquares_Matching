#include "matching.h"
#include <iostream>

int main()
{
    std::string left_path = "C:/Users/10527/Desktop/shuzisheyingceliang/LeastSquares_Matching/img/uavl_epi.jpg";
    std::string right_path = "C:/Users/10527/Desktop/shuzisheyingceliang/LeastSquares_Matching/img/uavr_epi.jpg";
    matching match(left_path, right_path);
    match.set_params(15);
    match.set_centers(1000,1000,1000,1000);
    match.disp_windows();
    return 0;
}