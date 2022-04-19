//
// Created by welsh on 2022/4/19.
//这里是傅里叶变化

/**
 *
    What is a Fourier transform and why use it?
    How to do it in OpenCV?
    Usage of functions such as: copyMakeBorder() , merge() , dft() , getOptimalDFTSize() , log() and normalize() .



*/


#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
static void help(char ** argv)
{
    cout << endl
         <<  "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl
         <<  "The dft of an image is taken and it's power spectrum is displayed."  << endl << endl
         <<  "Usage:"                                                                      << endl
         << argv[0] << " [image_name -- default lena.jpg]" << endl << endl;
}
int DFT(int argc, char ** argv)
{
    //显示帮助信息
    help(argv);

    //导入图片
    const char* filename = argc >=2 ? argv[1] : "lena.jpg";
    Mat I = imread( samples::findFile( filename ), IMREAD_GRAYSCALE); //以灰度图形式读入
    if( I.empty()){
        cout << "Error opening image" << endl;
        return EXIT_FAILURE;
    }

    //getOptimalDFTSize()返回这个最佳尺寸，我们可以使用copyMakeBorder()函数来扩展图像的边框.
    Mat padded;                            //将输入图像扩大到最佳尺寸
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // 在边框上添加零值
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    //复数空间
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         //用零添加到扩展的另一个平面

    //离散傅里叶变化计算
    dft(complexI, complexI);            // this way the result may fit in the source matrix
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))


    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude

    Mat magI = planes[0];
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    imshow("Input Image"       , I   );    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();
    return 0;
}