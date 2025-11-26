#include "matching.h"

matching::matching(std::string left_path, std::string right_path)
{
    left_img = cv::imread(left_path, cv::IMREAD_GRAYSCALE);
    right_img = cv::imread(right_path, cv::IMREAD_GRAYSCALE);
    W = left_img.cols;
    H = right_img.rows;
}

void matching::set_params(int size)
{
    window_size = size;
    left_window = cv::Mat::zeros(window_size, window_size, CV_8UC1);
    right_window = cv::Mat::zeros(window_size, window_size, CV_8UC1);
}

void matching::choose_centers(int left_x, int left_y, int right_x, int right_y)
{
    int k = window_size/2;
    for(int i = -k; i<k+1; i++) //x
    {
        for(int j = -k; j<k+1;j++) //y
        {
            left_window.at<uchar>(j+k, i+k) = left_img.at<uchar>(left_y+j, left_x+i);
            right_window.at<uchar>(j+k, i+k) = right_img.at<uchar>(right_y+j, right_x+i);
        }
    }
}