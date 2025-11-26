#include "opencv2/opencv.hpp"
#include <string>

class matching
{
    private:
    cv::Mat left_img;
    cv::Mat right_img;
    int window_size = 15;
    cv::Mat left_window;
    cv::Mat right_window;
    int W,H;
    int leftx, lefty, rightx, righty;

    public:
    matching(std::string left_path, std::string right_path);
    void set_params(int window_size);
    void set_centers(int left_x, int left_y, int right_x, int right_y); //construct windows
    void disp_windows();
};