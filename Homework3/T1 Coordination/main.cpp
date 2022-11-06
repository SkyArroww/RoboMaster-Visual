#include <iostream>
#include <assert.h>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>



const std::vector<cv::Point3d> PW_BIG{                      // 灯条坐标，单位：m
                                         {-0.115, 0.0265, 0.},
                                         {-0.115, -0.0265, 0.},
                                         {0.115, -0.0265, 0.},
                                         {0.115, 0.0265, 0.}
                                        };

cv::Mat cam_mat;
cv::Mat dst_coeffs;

int main(){
    cv::FileStorage reader("/home/lidox/VScode Projects/Homework3/T1 Coordination/hw3_t1/hw3_t1/f_mat_and_c_mat.yml", cv::FileStorage::READ);
    reader["F"] >> cam_mat;
    reader["C"] >> dst_coeffs;
    
    // cv::Mat src;
    // src = cv::imread("/home/lidox/VScode Projects/Homework3/T1 Coordination/hw3_t1/hw3_t1/hero.jpg");
    

    std::vector<cv::Point2f> corners;
    corners.push_back(cv::Point2f(575.508, 282.175));
    corners.push_back(cv::Point2f(573.93, 331.819));
    corners.push_back(cv::Point2f(764.518, 337.652));
    corners.push_back(cv::Point2f(765.729, 286.741));

    

    cv::Mat rvec, tvec;
    cv::solvePnP(PW_BIG, corners, cam_mat, dst_coeffs, rvec, tvec);
    
    // std::cout<<tvec<<std::endl;
    // std::cout<<tvec.size<<std::endl;
    
    
    Eigen::Vector3d cam_w;
    Eigen::Vector4d w4;
    cv::cv2eigen(tvec, cam_w);
    
    Eigen::Quaterniond q = {-0.0816168, 0.994363, -0.0676645, -0.00122528};

    Eigen::Matrix3d rot_c_to_w = q.matrix();

    // std::cout << cam_w <<std::endl;
    
    Eigen::Vector3d res = rot_c_to_w * cam_w;
    std::cout << res <<std::endl;
    return 0;
}
/*
[imu]
0.994363i + -0.0676645j + -0.00122528k + -0.0816168
[armor]
575.508 282.175
573.93 331.819
764.518 337.652
765.729 286.741
*/