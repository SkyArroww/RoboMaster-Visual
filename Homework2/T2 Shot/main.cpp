#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Geometry>
#include <iostream>
#include <assert.h>

int n;
Eigen::Matrix4d converter;
Eigen::Matrix<double, 3, 4> cam_f;


void Init();

int main(){
    Init();

    freopen("../points.txt", "r", stdin);
    std::cin >> n;

    Eigen::Vector4d w4;
    double x, y, z;
    cv::Mat res(720,1280,CV_8UC3);
    // std::cout<<n<<std::endl;
    for(int i=1; i <= n; ++i){
        std::cin >> x >> y >> z;
        w4 << x, y, z, 1.;

        Eigen::Vector4d c4 = converter * w4;
        Eigen::Vector3d u3 = cam_f * c4;
        u3 /= u3(2, 0);
        
        cv::Point2d centre = cv::Point2d(u3(0,0), u3(1,0));
        cv::circle(res, centre, 1, cv::Scalar(255, 255, 255), 1);
        // std::cout << centre.x << ' '<< centre.y << std::endl;
    }
    
    
    cv::imshow("res",res);
    cv::imwrite("../JiaoLong.jpg",res);
    cv::waitKey(0);
    std::fclose(stdin);
    return 0;
}

//-0.5 + 0.5i + 0.5j + -0.5k
/*
400., 0., 190., 0.,

0., 400., 160., 0.,

0., 0., 1., 0.;
*/

void Init(){
    double x_cam_w = 2., y_cam_w = 2., z_cam_w = 2.;
    Eigen::Quaterniond q={-0.5, 0.5, 0.5, -0.5};

    converter = [&q, &x_cam_w, &y_cam_w, &z_cam_w](){
        Eigen::Vector3d cam_w = {x_cam_w, y_cam_w, z_cam_w};
        Eigen::Matrix3d rot_c_to_w = q.matrix();
        
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        converter.block(0, 0, 3 ,3) = rot_c_to_w.transpose().cast<double>();

        converter.block(0, 3, 3, 1) = -rot_c_to_w.transpose().cast<double>() * cam_w;
        converter(3, 3) = 1.;
        return converter;
    }();
    
    cam_f << 400., 0.  , 190., 0.,
             0.  , 400., 160., 0.,
             0.  , 0.  , 1.  , 0.;

    return;
}