#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>

cv::Mat Channels_Sub(const cv::Mat &src);
cv::Mat HSV(const cv::Mat &src);
void draw(cv::Mat &src, const cv::Mat &msk);

int main(int argc, char *argv[]){
    cv::Mat src = cv::imread("../Apple.png");
    assert(src.channels() == 3);

    cv::Mat res_rgb = Channels_Sub(src);
    cv::Mat res_hsv = HSV(src);

    // cv::imshow("RGB", res_rgb);
    // cv::imshow("HSV", res_hsv);
    // cv::waitKey(0);
    
    draw(src, res_rgb);
    // cv::imwrite("Apple_hsv.jpg", src);
    imshow("Apple_res", src);
    cv::waitKey(0);
    return 0;
}

//多通道相减
cv::Mat Channels_Sub(const cv::Mat& src){ 
    cv::Mat channels[3];
    cv::split(src,channels);

    channels[1] = channels[2] - channels[1]; //  G -> R - G
    cv::Mat norm_src;
    cv::merge(channels, 3, norm_src);

    cv::Mat part_red, part_brown;
    cv::inRange(norm_src, cv::Scalar(0, 70, 110), cv::Scalar(180, 255, 255), part_red);
    cv::inRange(norm_src, cv::Scalar(0, 15, 90), cv::Scalar(40, 50, 140), part_brown);

    cv::Mat msk = part_red | part_brown;

    cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8,8));
    cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(10,10));
    cv::Mat res;

    cv::morphologyEx(msk, res, cv::MORPH_CLOSE, kernel1);
    cv::morphologyEx(res, res, cv::MORPH_OPEN, kernel2);
    return res;
}

//HSV
cv::Mat HSV(const cv::Mat& src){
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    cv::Mat hsv_red1, hsv_red2,  hsv_yellow;
    cv::inRange(hsv, cv::Scalar(0, 43, 46), cv::Scalar(10, 255, 255), hsv_red1);
    cv::inRange(hsv, cv::Scalar(156, 43, 46), cv::Scalar(180, 255, 255), hsv_red2);
    cv::inRange(hsv, cv::Scalar(10, 43, 30), cv::Scalar(20, 255, 255), hsv_yellow);

    cv::Mat hsv_red = cv::Mat::ones(cv::Size(src.cols, src.rows), CV_8UC1);
    cv::Mat ones_mat = cv::Mat::ones(cv::Size(src.cols, src.rows), CV_8UC1);

    hsv_red = 255 * (hsv_red - (hsv_red - hsv_red1/255).mul(hsv_red - hsv_red2/255));
    cv::Mat hsv_result = 255 * (ones_mat - (ones_mat - hsv_red/255).mul(ones_mat - hsv_yellow/255));

    cv::Mat Morph_Result1, Morph_Result2, Morph_Result3;
    cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20,20));
    cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(50,50));
    cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40,40));

    cv::morphologyEx(hsv_result, Morph_Result1, cv::MORPH_OPEN, kernel1);
    cv::morphologyEx(Morph_Result1, Morph_Result2, cv::MORPH_CLOSE, kernel2);
    cv::morphologyEx(Morph_Result2, Morph_Result3, cv::MORPH_OPEN, kernel3);
    return Morph_Result3;
}

//draw_contours
void draw(cv::Mat &src, const cv::Mat &msk){
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(msk, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    int mx_id = -1;
    double mx_area = 0;
    for(int i = 0; i < contours.size(); ++i){
        double area = cv::contourArea(contours[i]);
        if(area > mx_area){
            mx_id = i;
            mx_area = area;
        }
    }

    if(mx_id == -1) return;

    
    cv::Rect rect = cv::boundingRect(contours[mx_id]);
    cv::drawContours(src, contours, mx_id, cv::Scalar(255, 255, 0), 2);
    cv::rectangle(src, rect, cv::Scalar(255, 255, 0), 2);
    return;
}