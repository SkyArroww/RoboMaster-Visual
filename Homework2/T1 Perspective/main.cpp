#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>

cv::Mat Affine(const cv::Mat &src);
cv::Mat Perspective(const cv::Mat &src);
int main(){
    cv::Mat src = cv::imread("../taxi.png");
    
    cv::Mat Affine_src = Affine(src);
    cv::Mat Perspective_src = Perspective(src);



    cv::imwrite("Affine.png", Affine_src);
    cv::imwrite("Perspective.png", Perspective_src);
    cv::imshow("src", src);
    cv::imshow("Affine", Affine_src);
    cv::imshow("Perspective", Perspective_src);
    cv::waitKey(0);
    return 0;
}
//(737, 409)  (961, 411)  (746, 490)  (972, 490)
//(737, 409)  (961, 409)  (737, 492)  (961 492)
cv::Mat Affine(const cv::Mat &src){
    cv::Point2f p1[5], p2[5];
    
    p1[0] = cv::Point2f(737, 409); p1[1] = cv::Point2f(746, 490); p1[2] = cv::Point2f(961, 411);
    p2[0] = cv::Point2f(737, 409); p2[1] = cv::Point2f(737, 492); p2[2] = cv::Point2f(961, 409);

    cv::Mat Affine_Matrix = cv::getAffineTransform(p1, p2);

    cv::Mat res;
    cv::warpAffine(src, res, Affine_Matrix, src.size());
    
    // cv::line(src, p1[0], p1[1], cv::Scalar(255, 255, 255), 1);
    // cv::line(res, p2[1], p2[2], cv::Scalar(255, 255, 255), 1);

    res = res(cv::Rect(737,409,220,80));
    return res;
}

cv::Mat Perspective(const cv::Mat &src){
    cv::Point2f p1[5], p2[5];
    
    p1[0] = cv::Point2f(737, 409); p1[1] = cv::Point2f(746, 490); p1[2] = cv::Point2f(961, 411); p1[3] = cv::Point2f(972, 490);
    p2[0] = cv::Point2f(737, 409); p2[1] = cv::Point2f(737, 492); p2[2] = cv::Point2f(961, 409); p2[3] = cv::Point2f(961, 492);

    cv::Mat Perspective_Matrix = cv::getPerspectiveTransform(p1,p2);

    cv::Mat res;
    cv::warpPerspective(src, res, Perspective_Matrix, src.size());

    res = res(cv::Rect(737,409,220,80));
    return res;
}