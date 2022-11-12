#include <iostream>
#include <assert.h>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


int main(){
    cv::Mat src1 = cv::imread("/home/lidox/VScode Projects/Homeworl4/T1 Match/hw4_t1/9407.png");
    cv::Mat src2 = cv::imread("/home/lidox/VScode Projects/Homeworl4/T1 Match/hw4_t1/9408.png"); 
    
    cv::Mat gray1, gray2;
    cv::cvtColor(src1, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(src2, gray2, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();

    std::vector<cv::KeyPoint> feature_points1, feature_points2;
    orb -> detect(gray1, feature_points1);
    orb -> detect(gray2, feature_points2);

    cv::Mat descriptor1, descriptor2;
    orb -> compute(gray1, feature_points1, descriptor1);
    orb -> compute(gray2, feature_points2, descriptor2);

    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> pairs;
    matcher.match(descriptor1, descriptor2, pairs);

    cv::Mat drawer1;
    cv::drawMatches(src1, feature_points1, src2, feature_points2, pairs, drawer1);
    cv::imwrite("../Matches_pre.jpg", drawer1);
    // cv::imshow("res1", drawer1);
    
    std::vector<double> dist;
    for(const cv::DMatch &m : pairs){
        dist.push_back(m.distance);
    }
  std::sort(dist.begin(), dist.end());
  double min_dist = dist[0.5 * dist.size()];

    std::vector<cv::DMatch> v;
    for(const cv::DMatch &m : pairs) {
        if(m.distance < min_dist) {
            v.push_back(m);
     }
    }
    

    cv::Mat drawer2;
    cv::drawMatches(src1, feature_points1, src2, feature_points2, v, drawer2);
    cv::imwrite("../Matches_good.jpg", drawer2);
    // cv::imshow("res2", drawer2);
    // cv::waitKey(0);
    return 0;
}