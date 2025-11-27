#include "matching.h"

matching::matching(std::string left_path, std::string right_path)
{
    left_img = cv::imread(left_path, cv::IMREAD_GRAYSCALE);
    right_img = cv::imread(right_path, cv::IMREAD_GRAYSCALE);
    W = left_img.cols;
    H = right_img.rows;
}

void matching::set_params(int size = 15)
{
    window_size = size;
    left_window = cv::Mat::zeros(window_size, window_size, CV_32F);
    right_window = cv::Mat::zeros(window_size, window_size, CV_32F);
    g2_dx = cv::Mat::zeros(window_size-2, window_size-2, CV_32F);
    g2_dy = cv::Mat::zeros(window_size-2, window_size-2, CV_32F);
}

void matching::set_centers(int left_x, int left_y, int right_x, int right_y)
{
    leftx = left_x;
    lefty = left_y;
    rightx = right_x;
    righty = right_y; //record the centers
    int k = (int)(window_size/2);
    for(int i = -k; i<k+1; i++) //x
    {
        for(int j = -k; j<k+1;j++) //y
        {
            left_window.at<float>(j+k, i+k) = left_img.at<uchar>(left_y+j, left_x+i);
            right_window.at<float>(j+k, i+k) = right_img.at<uchar>(right_y+j, right_x+i);
        }
    }
}

void matching::disp_windows()
{
    cv::imshow("Left",left_window);
    cv::imshow("Right",right_window);
    cv::waitKey(0);
}

void matching::params_init()
{
    X = Matrix(8,1); //h0,h1,a0,a1,a2,b0,b1,b2
    X.SetMatrix_ele(0,0,0); //h0
    X.SetMatrix_ele(1,0,1); //h1
    X.SetMatrix_ele(2,0,0); //a0
    X.SetMatrix_ele(3,0,1); //a1
    X.SetMatrix_ele(4,0,0); //a2
    X.SetMatrix_ele(5,0,0); //b0
    X.SetMatrix_ele(6,0,0); //b1
    X.SetMatrix_ele(7,0,1); //b2

    B = Matrix((window_size-2)*(window_size-2), 8);
    L = Matrix((window_size-2)*(window_size-2), 1);
}

float matching::sample_img(cv::Mat img, double x, double y)
{
    int width = img.cols;
    int height = img.rows;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > width - 1) 
    {
        x = width - 1;
    }
    if (y > height - 1) 
    {
        y = height - 1;
    }

    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = std::min(x0 + 1, width - 1);
    int y1 = std::min(y0 + 1, height - 1);

    double dx = x - x0;
    double dy = y - y0;

    double I00 = img.at<uchar>(y0, x0);
    double I10 = img.at<uchar>(y0, x1);
    double I01 = img.at<uchar>(y1, x0);
    double I11 = img.at<uchar>(y1, x1);

    double I = I00 * (1 - dx) * (1 - dy) +
               I10 * dx * (1 - dy) +
               I01 * (1 - dx) * dy +
               I11 * dx * dy;
    return static_cast<float>(clamp(I, 0.0, 255.0));
}

void matching::get_g2()
{
    double a0 = X.getMatrix_ele(2,0);
    double a1 = X.getMatrix_ele(3,0);
    double a2 = X.getMatrix_ele(4,0);
    double b0 = X.getMatrix_ele(5,0);
    double b1 = X.getMatrix_ele(6,0);
    double b2 = X.getMatrix_ele(7,0);
    int k = (int)(window_size/2);
    for(int i = -k; i<k+1; i++)
    {
        for(int j = -k; j<k+1;j++)
        {
            int x = rightx + i;
            int y = righty + j; //变形前的像素点坐标
            double x2 = a0 + a1*x + a2*y;
            double y2 = b0 + b1*x + b2*y; //变形后的像素点坐标
            right_window.at<float>(j+k, i+k) = sample_img(right_img, x2, y2);
        }
    }
    g2 = right_window.clone();
}

void matching::radioCorrection()
{
    double h0 = X.getMatrix_ele(0,0);
    double h1 = X.getMatrix_ele(1,0);

    int k = (int)(window_size/2);
    for(int i = -k; i<k+1; i++)
    {
        for(int j = -k; j<k+1;j++)
        {
            float val = right_window.at<float>(j+k, i+k);
            double corrected = h0 + h1 * val;
            right_window.at<float>(j+k, i+k) = static_cast<float>(clamp(corrected, 0.0, 255.0));
        }
    }
}

void matching::get_dg()
{
    for(int i = 0; i<window_size-2; i++)
    {
        for(int j = 0;j<window_size-2;j++)
        {
            g2_dx.at<float>(j,i) = 0.5*(g2.at<float>(j+1,i+2)-g2.at<float>(j+1,i));
            g2_dy.at<float>(j,i) = 0.5*(g2.at<float>(j+2,i+1)-g2.at<float>(j,i+1));
        }
    }
}

void matching::construct_matrices()
{
    double h1 = X.getMatrix_ele(1,0);
    int pix_counter = 0;
    int k = (int)((window_size-2)/2);
    for(int i = 0;i<window_size-2; i++)
    {
        for(int j = 0;j<window_size-2; j++)
        {
            int x = rightx - k + i;
            int y = righty - k + j;
            B.SetMatrix_ele(pix_counter, 0, 1);
            B.SetMatrix_ele(pix_counter, 1, g2.at<float>(j+1, i+1));

            B.SetMatrix_ele(pix_counter, 2, h1*g2_dx.at<float>(j,i));
            B.SetMatrix_ele(pix_counter, 3, h1*x*g2_dx.at<float>(j,i));
            B.SetMatrix_ele(pix_counter, 4, h1*y*g2_dx.at<float>(j,i));

            B.SetMatrix_ele(pix_counter, 5, h1*g2_dy.at<float>(j,i));
            B.SetMatrix_ele(pix_counter, 6, h1*x*g2_dy.at<float>(j,i));
            B.SetMatrix_ele(pix_counter, 7, h1*y*g2_dy.at<float>(j,i));

            float dl = left_window.at<float>(j+1, i+1) - right_window.at<float>(j+1, i+1);
            L.SetMatrix_ele(pix_counter, 0, dl);
            pix_counter++;
        }
    }
}