#include <iostream>
#include <assert.h>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "VO.h"

int main(){
    cv::Mat img1 = cv::imread("/home/lidox/VScode Projects/Homeworl4/T2 Depth/hw4_t2/stereo-data/0_orig.jpg");
    cv::Mat dpt1 = cv::imread("/home/lidox/VScode Projects/Homeworl4/T2 Depth/hw4_t2/stereo-data/0_dpt.tiff", cv::IMREAD_ANYDEPTH);
    cv::Mat img2 = cv::imread("/home/lidox/VScode Projects/Homeworl4/T2 Depth/hw4_t2/stereo-data/1_orig.jpg");
    cv::Mat dpt2 = cv::imread("/home/lidox/VScode Projects/Homeworl4/T2 Depth/hw4_t2/stereo-data/1_dpt.tiff", cv::IMREAD_ANYDEPTH);

    cv::FileStorage params("/home/lidox/VScode Projects/Homeworl4/T2 Depth/hw4_t2/camera.yaml", cv::FileStorage::READ);
	cv::Mat K = params["K"].mat();

    
    cv::Mat R, t, rvec, tvec; 
    find_PnP(img1, dpt1, img2, dpt2, K, rvec, tvec);
    find_use_E(img1, dpt1, img2, dpt2, K, R, t);

    process_Stitch_project(img1, dpt1, img2, dpt2, K, R, tvec, "result");

    return 0;
}