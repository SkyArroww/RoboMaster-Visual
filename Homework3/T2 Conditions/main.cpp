#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Dense>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <assert.h>
using namespace cv;
using namespace Eigen;

std::vector <double> data;
int main(){

    freopen("/home/lidox/VScode Projects/Homework3/T2 Conditions/hw3_t2/hw3_t2/dollar.txt", "r", stdin);
   for(int i = 0, x; i < 30; ++i){
        std::cin >> x;
        data.push_back(x);
   }

    const int Z_N = 1, X_N = 2;
    Matrix<double, X_N, 1> X;
    Matrix<double, X_N, X_N> A;
    Matrix<double, X_N, X_N> P;
    Matrix<double, X_N, X_N> R;
    Matrix<double, X_N, Z_N> K;
    Matrix<double, Z_N, X_N> C;
    Matrix<double, Z_N, Z_N> Q;

    X << data[0], 0;
    A << 1, 1, 0, 1;
    C << 1, 0;
    R << 2, 0, 0, 2;
    Q << 10;

    for (int i = 1; i < data.size(); i++){
        // 更新预测
        Matrix<double, X_N, 1> X_k = A * X;
        P = A * P * A.transpose() + R;
        // 更新观测
        K = P * C.transpose() * (C * P * C.transpose() + Q).inverse();
        Matrix<double, Z_N, 1> Z{data[i]};
        X = X_k + K * (Z - C * X_k);
        P = (Matrix<double, X_N, X_N>::Identity() - K * C) * P;

        std::cout << "step " << i << ": " << X[1] << std::endl;
    }

    std::cout << "final speed: " << X[1] << std::endl;
    for (int i = 1; i < 10; ++i){
        X[0] += X[1];
        std::cout << "day " << i << ": " << X[0] << std::endl;
    }
    return 0;
}