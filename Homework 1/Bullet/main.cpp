#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>
#include <vector>

bool JudgeContourByArea(const std::vector<cv::Point> &contour){
    if(cv::contourArea(contour) > 200) return true;
    return false;
}

int main(){
    cv::VideoCapture capture("../bullet_video.mp4");
    cv::Mat src;
    capture.read(src);

    // cv::VideoWriter writer("../bullet_detection.avi",
    //                         cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 50,
    //                         cv::Size(src.cols, src.rows), true);
    
    // cv::Mat src = cv::imread("../bullet.jpg");
    // assert(src.channels() == 3);

    do{
        assert(src.channels() == 3);
        
        cv::Mat rgb_green;
        cv::inRange(src, cv::Scalar(60, 90, 40), cv::Scalar(255, 255, 160), rgb_green);
        
        cv::Mat Morph1, Morph2, Morph3;
        cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,1));
        cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
        cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
        cv::morphologyEx(rgb_green, Morph1, cv::MORPH_ERODE, kernel1);
        cv::morphologyEx(Morph1, Morph2, cv::MORPH_DILATE, kernel2);
        cv::morphologyEx(Morph2, Morph3, cv::MORPH_CLOSE, kernel3);

        // cv::imshow("src",Morph3);
        // cv::waitKey(0);
        // return 0;

        std::vector<std::vector<cv::Point>> contour;
        std::vector<cv::Vec4i> hierachy;
        cv::findContours(Morph3, contour, hierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

        if(contour.size()==0) continue;
        for(int i = 0; i != -1; i = hierachy[i][0]){
            // std::printf("p %d\n",i);
            if(JudgeContourByArea(contour[i])){
                cv::RotatedRect Res_Rect = cv::minAreaRect(contour[i]);
                
                cv::Point2f pts[4] = { cv::Point(0,0) };
                Res_Rect.points(pts);
                line(src, pts[0], pts[1], {0,0,255}, 1, 8);
                line(src, pts[1], pts[2], {0,0,255}, 1, 8);
                line(src, pts[2], pts[3], {0,0,255}, 1, 8);
                line(src, pts[3], pts[0], {0,0,255}, 1, 8);

                cv::Point center = cv::Point(int(pts[0].x + pts[2].x)/2, int(pts[0].y + pts[2].y)/2);
                cv::circle(src, center, 2, {255,255,255}, 2);
            }
        }

        // writer << src;
        cv::imshow("morph", Morph3);
        cv::imshow("src", src);
        cv::waitKey(100);
    
    }while(capture.read(src));
    // writer.release();
    // puts("999");
    // cv::imshow("src", src);
    // cv::imshow("green", rgb_green);
    // cv::imshow("Morph", Morph);
    // cv::imshow("Canny", Canny_res);
    // cv::imwrite("Bullet_Detection.jpg", src);
    // cv::waitKey(0);
    return 0;
}