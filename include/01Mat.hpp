//
// Created by welsh on 2022/4/3.
//

///@brief 这里是用来介绍Mat的属性的

/**
 * 具有两个数据部分的类：矩阵头（包含诸如矩阵大小、用于存储的方法、存储的矩阵的地址等信息）和指向包含像素值（根据选择的存储方法采用任何维度）。矩阵头大小是恒定的，但是矩阵本身的大小可能因图像而异。
 *
 * OpenCV 底层是用的cpp的智能指针。赋值运算符和复制构造函数只浅复制。
 * OpenCV 使用了引用计数系统。这个想法是每个Mat对象都有自己的标头，但是通过让它们的矩阵指针指向相同的地址，可以在两个Mat对象之间共享一个矩阵。此外，复制运算符只会复制标题和指向大矩阵的指针，而不是数据本身。
 *
 * OpenCV的深复制
 * 有时你也想复制矩阵本身，所以 OpenCV 提供了cv::Mat::clone()和cv::Mat::copyTo()函数。
 *
 *
 *创建方法
 *      1、cv::Mat::Mat构造函数,大概有27个构造函数，具体使用可以参考API文档
 *      2、使用 C/C++ 数组并通过构造函数进行初始化
 *      3、cv::Mat::create函数
 *      4、MATLAB 样式初始化程序：cv::Mat::zeros、cv::Mat::ones、cv::Mat::eye。指定要使用的大小和数据类型
 *      5、对于小型矩阵，您可以使用逗号分隔的初始化程序或初始化程序列表
 *      6、为现有的Mat对象和cv::Mat::clone或cv::Mat::copyTo创建
 *
 *      可以使用cv::randu()函数填充具有随机值的矩阵。您需要为随机值提供下限和上限
 *
 *
 *
 */



#include "opencv2/core.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void help()
{
    cout
            << "\n---------------------------------------------------------------------------" << endl
            << "This program shows how to create matrices(cv::Mat) in OpenCV and its serial"
            << " out capabilities"                                                             << endl
            << "That is, cv::Mat M(...); M.create and cout << M. "                             << endl
            << "Shows how output can be formatted to OpenCV, python, numpy, csv and C styles." << endl
            << "Usage:"                                                                        << endl
            << "./mat_the_basic_image_container"                                               << endl
            << "-----------------------------------------------------------------------------" << endl
            << endl;
}

void matTest(int,char**)
{
    help();
    // create by using the constructor
    //! [constructor]
    Mat M(2,2, CV_8UC3, Scalar(0,0,255));
    cout << "M = " << endl << " " << M << endl << endl;
    //! [constructor]

    // create by using the create function()
    //! [create]
    M.create(4,4, CV_8UC(2));
    cout << "M = "<< endl << " "  << M << endl << endl;
    //! [create]

    // create multidimensional matrices
    //! [init]
    int sz[3] = {2,2,2};
    Mat L(3,sz, CV_8UC(1), Scalar::all(0));
    //! [init]

    // Cannot print via operator <<

    // Create using MATLAB style eye, ones or zero matrix
    //! [matlab]
    Mat E = Mat::eye(4, 4, CV_64F);
    cout << "E = " << endl << " " << E << endl << endl;
    Mat O = Mat::ones(2, 2, CV_32F);
    cout << "O = " << endl << " " << O << endl << endl;
    Mat Z = Mat::zeros(3,3, CV_8UC1);
    cout << "Z = " << endl << " " << Z << endl << endl;
    //! [matlab]

    // create a 3x3 double-precision identity matrix
    //! [comma]
    Mat C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cout << "C = " << endl << " " << C << endl << endl;
    //! [comma]
    // do the same with initializer_list
#ifdef CV_CXX11
    //! [list]
    C = (Mat_<double>({0, -1, 0, -1, 5, -1, 0, -1, 0})).reshape(3);
    cout << "C = " << endl << " " << C << endl << endl;
    //! [list]
#endif
    //! [clone]
    Mat RowClone = C.row(1).clone();
    cout << "RowClone = " << endl << " " << RowClone << endl << endl;
    //! [clone]

    // Fill a matrix with random values
    //! [random]
    Mat R = Mat(3, 2, CV_8UC3);
    randu(R, Scalar::all(0), Scalar::all(255));
    //! [random]

    // Demonstrate the output formatting options
    //! [out-default]
    cout << "R (default) = " << endl <<        R           << endl << endl;
    //! [out-default]
    //! [out-python]
    cout << "R (python)  = " << endl << format(R, Formatter::FMT_PYTHON) << endl << endl;
    //! [out-python]
    //! [out-numpy]
    cout << "R (numpy)   = " << endl << format(R, Formatter::FMT_NUMPY ) << endl << endl;
    //! [out-numpy]
    //! [out-csv]
    cout << "R (csv)     = " << endl << format(R, Formatter::FMT_CSV   ) << endl << endl;
    //! [out-csv]
    //! [out-c]
    cout << "R (c)       = " << endl << format(R, Formatter::FMT_C     ) << endl << endl;
    //! [out-c]

    //! [out-point2]
    Point2f P(5, 1);
    cout << "Point (2D) = " << P << endl << endl;
    //! [out-point2]

    //! [out-point3]
    Point3f P3f(2, 6, 7);
    cout << "Point (3D) = " << P3f << endl << endl;
    //! [out-point3]

    //! [out-vector]
    vector<float> v;
    v.push_back( (float)CV_PI);   v.push_back(2);    v.push_back(3.01f);
    cout << "Vector of floats via Mat = " << Mat(v) << endl << endl;
    //! [out-vector]

    //! [out-vector-points]
    vector<Point2f> vPoints(20);
    for (size_t i = 0; i < vPoints.size(); ++i)
        vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
    cout << "A vector of 2D Points = " << vPoints << endl << endl;
    //! [out-vector-points]
    return ;
}


