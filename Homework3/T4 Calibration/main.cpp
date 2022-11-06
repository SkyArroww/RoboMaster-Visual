#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <assert.h>

const int tot_img = 20;
const int board_w = 9;
const int board_h = 6;
const int board_n = 54;

int img_cnt = 0;
cv::Size img_size;
std::vector< std::vector< cv::Point2f > > point_pix_pos;
cv::Mat dst_coeffs(1, 5, CV_32FC1, cv::Scalar::all(0));
cv::Mat cam_mat(3, 3, CV_32FC1, cv::Scalar::all(0));

void PnP();
void Find_Corners();
void Camera_Calibration();
cv::Mat Brighten(cv::Mat &src);
void Fix();
int main(){
    Find_Corners();
    Camera_Calibration();
    Fix();
    // PnP();
    return 0;
}
cv::Mat Brighten(cv::Mat &src, int contrast, int bright){
    cv::Mat res;
    res.create(src.size(), src.type());
    for(int i = 0;i < src.rows;++i){
        for(int j = 0;j < src.cols;++j){
            res.at< cv::Vec3b >(i, j)[0] = cv::saturate_cast< uchar >(src.at< cv::Vec3b >(i, j)[0] * contrast*0.01 + bright);
            res.at< cv::Vec3b >(i, j)[1] = cv::saturate_cast< uchar >(src.at< cv::Vec3b >(i, j)[1] * contrast*0.01 + bright);
            res.at< cv::Vec3b >(i, j)[2] = cv::saturate_cast< uchar >(src.at< cv::Vec3b >(i, j)[2] * contrast*0.01 + bright);
        }
    }
    return res;
}
void Find_Corners(){
    cv::Mat gray_img, drawn_img;
    cv::Size board_size(9, 6);
    std::vector< cv::Point2f > point_pix_pos_buf;
    
    bool f1 = false, flag;

    for(int i = 0; i < tot_img; ++i){
        std::string dir;
        dir = std::__cxx11::to_string(i).append("_orig.jpg");
        cv::Mat src0 = cv::imread("../hw3_t4/datapack/calib2/" + dir, 1);
        
        cv::Mat src = Brighten(src0, 200, 20); //加大对比度, 便于处理过暗图片
        // cv::Mat src = src0;
        
        if(!f1){
            img_size.width = src.cols;
            img_size.height = src.rows;
            f1 = true;
        }

        flag = findChessboardCorners(src, board_size, point_pix_pos_buf);
        
        if(flag && point_pix_pos_buf.size() == board_n){
            ++img_cnt;
            cv::cvtColor(src, gray_img, cv::COLOR_BGR2GRAY);
            cv::find4QuadCornerSubpix(gray_img, point_pix_pos_buf, cv::Size(5,5));  //亚像素精化
            point_pix_pos.push_back(point_pix_pos_buf);
            
            
            drawn_img = src.clone();
            cv::drawChessboardCorners(drawn_img, board_size, point_pix_pos_buf, flag);
            // cv::imshow("Corners", drawn_img);
            // cv::waitKey(20);
        }
        else{
            std::cout<<"id: "<<i<<' '<<"failed!"<<std::endl;
        }

        point_pix_pos_buf.clear();
    }
}

void Camera_Calibration(){
    cv::Size square_size(10, 10);
    std::vector< std::vector< cv::Point3f > > point_grid_pos;
    std::vector< cv::Point3f > point_grid_pos_buf;
    std::vector< int > point_cnt;
    std::vector< cv::Mat > rvecs;
    std::vector< cv::Mat > tvecs;

    for(int i = 0 ;i < img_cnt ;++i){
        for(int j = 0 ;j < board_h ; ++j){
            for(int k = 0; k < board_w; ++k){
                cv::Point3f pt;
                pt.x = k * square_size.width;
                pt.y = j * square_size.height;
                pt.z = 0;
                point_grid_pos_buf.push_back(pt);
            }
        }
        point_grid_pos.push_back(point_grid_pos_buf);
        point_grid_pos_buf.clear();
        point_cnt.push_back( board_h * board_w);
    }
    std::cout << cv::calibrateCamera(point_grid_pos, point_pix_pos, img_size, cam_mat, dst_coeffs, rvecs, tvecs) << std::endl;
    std::cout << cam_mat << std::endl << dst_coeffs << std::endl;
    return;
}

void Fix(){
    for(int i = 0; i < tot_img; ++i){
        std::string dir;
        dir = std::__cxx11::to_string(i).append("_orig.jpg");
        cv::Mat src = cv::imread("../hw3_t4/datapack/calib2/" + dir, 1);
        
        
        cv::Mat res;
        cv::undistort(src, res, cam_mat, dst_coeffs);
        res = Brighten(res, 200, 20);
        cv::imshow("src", src);
        cv::imshow("res", res);
        cv::waitKey(500);
    }
    return;
}

void PnP(){
    bool flag;
    cv::Mat rvec, tvec;
    cv::Size board_size(9, 6);
    std::vector< cv::Point2f> corners;
    std::vector< cv::Point3f> dst;
    std::vector< cv::Point2f > point_pix_pos_buf;

    for(int i = 0; i < img_cnt; ++i){

        std::string dir;
        dir = std::__cxx11::to_string(i).append("_orig.jpg");
        cv::Mat src0 = cv::imread("../hw3_t4/datapack/calib2/" + dir, 1);
        
        cv::Mat src = Brighten(src0, 200, 20);


        flag = findChessboardCorners(src, board_size, point_pix_pos_buf);
        if(!flag){
            std::cout<<"id: "<<i<<' '<<"failed!"<<std::endl;
            continue;
        }

        int tmp = point_pix_pos_buf.size();
        corners.push_back(point_pix_pos_buf[0]);
        corners.push_back(point_pix_pos_buf[8]);
        corners.push_back(point_pix_pos_buf[tmp - 9]);
        corners.push_back(point_pix_pos_buf[tmp - 1]);
        
        dst.push_back({0, 0, 0});
        dst.push_back({8, 0, 0});
        dst.push_back({0, 5, 0});
        dst.push_back({8, 5, 0});
        
        cv::solvePnP(dst, corners, cam_mat, dst_coeffs, rvec, tvec);

        std::cout << "t: " << std::endl << -tvec <<std::endl;

        cv::Mat drawer = src.clone();
        for(int j = 0; j < 4; ++j){
            cv::circle(drawer, corners[j], 5, cv::Scalar(0, 255, 255), 3);
        }

        // cv::imshow("src", src);
        // cv::imshow("corners", drawer);
        // cv::waitKey(2000);

        dst.clear();
        corners.clear();
        point_pix_pos_buf.clear();

    }
    return;
}