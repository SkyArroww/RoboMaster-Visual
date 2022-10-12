#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>
#include <vector>

cv::Mat Channels_Sub(const cv::Mat &src);
cv::Mat rgb_extract(const cv::Mat &src);
bool JudgeContourByArea(const std::vector<cv::Point> &contour);

int main(){
    cv::VideoCapture capture("../bullet_video.mp4");
    cv::Mat src;
    capture.read(src);

    cv::VideoWriter writer("../bullet_detection.avi",
                            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 50,
                            cv::Size(src.cols, src.rows), true);
    
    int T = 0, id = 0, lst_x, lst_y, now_x, now_y, xd, yd;
    do{
        ++T;
        assert(src.channels() == 3);
        cv::Mat res = Channels_Sub(src);
        
        std::vector<std::vector<cv::Point>> contour;
        std::vector<cv::Vec4i> hierachy;
        cv::findContours(res, contour, hierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

        if(contour.size()==0) continue;

        int mx_id = -1;
        double mx_area = 0;
        for(int i = 0; i < contour.size(); ++i){
            double area = cv::contourArea(contour[i]);
            if(area < 140) continue;
            if(area > mx_area){
                mx_id = i;
                mx_area = area;
            }
        }

        if(mx_id != -1){
            cv::RotatedRect Res_Rect = cv::minAreaRect(contour[mx_id]);
                
            cv::Point2f pts[4] = { cv::Point(0,0) };
            Res_Rect.points(pts);
            line(src, pts[0], pts[1], cv::Scalar{255, 255, 255}, 1, 8);
            line(src, pts[1], pts[2], cv::Scalar{255, 255, 255}, 1, 8);
            line(src, pts[2], pts[3], cv::Scalar{255, 255, 255}, 1, 8);
            line(src, pts[3], pts[0], cv::Scalar{255, 255, 255}, 1, 8);

            now_x = int(pts[0].x + pts[2].x)/2;
            now_y = int(pts[0].y + pts[2].y)/2;
            cv::Point center = cv::Point(now_x, now_y);
            cv::circle(src, center, 2, cv::Scalar{0, 0, 255}, 2);

            if(id == 0){
                ++id;
            }
            else{
                xd = now_x - lst_x; 
                yd = now_y - lst_y;
                if(sqrt(xd * xd + yd * yd) > 200) ++id;
            }
            lst_x = now_x;
            lst_y = now_y;

            cv::putText(src, "ID: " + std::to_string(id), cv::Point(center.x + 20, center.y + 20),
				cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 255), 2);
        }

        cv::putText(src, "Now: " + std::to_string(T), cv::Point(0, 20),
				cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 255), 2);
        writer << src;
        cv::imshow("res", res);
        cv::imshow("src", src);
        cv::waitKey(50);
    
    }while(capture.read(src));

    return 0;
}

cv::Mat Channels_Sub(const cv::Mat& src){ 
    cv::Mat channels[3];
    cv::split(src,channels);

    cv::Mat green_sub_blue = channels[1] - (channels[2] + channels[0])*0.4;
    
    cv::Mat norm_mat;
    cv::normalize(green_sub_blue, norm_mat, 0., 255., cv::NORM_MINMAX);

    cv::Mat res;
    cv::threshold(norm_mat, res, 100, 255, cv::THRESH_OTSU);

    cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(2,2));
    cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3));
    cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,1));
    cv::morphologyEx(res, res, cv::MORPH_DILATE, kernel1);
    cv::morphologyEx(res, res, cv::MORPH_CLOSE, kernel2);
    cv::morphologyEx(res, res, cv::MORPH_CLOSE, kernel3);
    return res;
}

cv::Mat rgb_extract(const cv::Mat &src){
    cv::Mat rgb_green;
    cv::inRange(src, cv::Scalar(50, 0, 100), cv::Scalar(200, 255, 150), rgb_green);
    
    cv::Mat Morph1, Morph2, Morph3;
    cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(2,2));
    cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    // cv::morphologyEx(rgb_green, Morph1, cv::MORPH_ERODE, kernel1);
    // cv::morphologyEx(Morph1, Morph2, cv::MORPH_DILATE, kernel2);
    cv::morphologyEx(rgb_green, Morph3, cv::MORPH_CLOSE, kernel3);
    cv::morphologyEx(Morph3, Morph3, cv::MORPH_CLOSE, kernel3);
    
    return Morph3;
}

bool JudgeContourByArea(const std::vector<cv::Point> &contour){
    if(cv::contourArea(contour) > 160) return true;
    return false;
}