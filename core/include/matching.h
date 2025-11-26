#include "opencv2/opencv.hpp"
#include <string>

class matching
{
    private:
    cv::Mat left_img;
    cv::Mat right_img;
    int window_size = 15;

    public:
    matching(std::string left_path, std::string right_path);
    void set_params(int window_size);
};