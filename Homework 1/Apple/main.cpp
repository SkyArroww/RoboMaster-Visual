#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>

int main(){
    cv::Mat src = cv::imread("../Apple.png");
    assert(src.channels() == 3);

    /*多通道相减
    cv::Mat channels[3];
    cv::split(src,channels);

    cv::Mat R_Sub_B = channels[2] - channels[1];

    cv::Mat Norm_Mat;
    cv::normalize(R_Sub_B, Norm_Mat, 0., 255., cv::NORM_MINMAX);

    cv::Mat Result;
    cv::threshold(Norm_Mat, Result, 100, 255, cv::THRESH_OTSU);

    cv::imshow("src", src);
    cv::imshow("norm", Norm_Mat);
    cv::imshow("result", Result);
    cv::waitKey(0);
    */

    //hsv
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


    cv::Mat Canny_Result;
    cv::Canny(Morph_Result3, Canny_Result, 50, 100, 3);

    cv::Rect Res_Rect = cv::boundingRect(Canny_Result);
    cv::rectangle(src, Res_Rect, (0,255,0), 5);
    cv::imshow("src", src);
    // cv::imshow("yellow", hsv_yellow);
    // cv::imshow("red", hsv_red);
    // cv::imshow("hsv", hsv_result);
    // cv::imshow("Morph1", Morph_Result1);
    // cv::imshow("Morph2", Morph_Result2);
    // cv::imshow("Morph3", Morph_Result3);
    cv::imshow("Canny", Canny_Result);
    cv::imwrite("Apple_Res.jpg", src);
    cv::waitKey(0);
    return 0;
}