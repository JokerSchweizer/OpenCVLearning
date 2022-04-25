/*

blur()    //均值模糊   一般用来处理图像的随机噪声；
GaussianBlur()    //高斯模糊
medianBlur()      // 中值模糊   一般用来处理图像的椒盐噪声；
bilateralFilter()     //双边滤波  可以去除无关噪声，同时保持较好的边缘信息

filter2D（）//2D 卷积   自定义核函数的模糊滤波




*/


#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
Mat src; Mat dst;
char window_name[] = "Smoothing Demo";


int display_caption( const char* caption );
int display_dst( int delay );
int SmoothTest( int argc, char ** argv )
{
    //创建了窗口
    namedWindow( window_name, WINDOW_AUTOSIZE );
    //读取数据
    const char* filename = argc >=2 ? argv[1] : "lena.jpg";
    src = imread( samples::findFile( filename ), IMREAD_COLOR );
    if (src.empty())
    {
        printf(" Error opening image\n");
        printf(" Usage:\n %s [image_name-- default lena.jpg] \n", argv[0]);
        return EXIT_FAILURE;
    }
    /////////////////////////////////////////////显示原始图像
    if( display_caption( "Original Image" ) != 0 )
    {
        return 0;
    }
    dst = src.clone();
    if( display_dst( DELAY_CAPTION ) != 0 )
    {
        return 0;
    }

    //////////////////////////////////////显示blur图像
    if( display_caption( "Homogeneous Blur" ) != 0 )
    {
        return 0;
    }
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        blur( src, dst, Size( i, i ), Point(-1,-1) );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }

    /////////////////////////////////////高斯模糊
    if( display_caption( "Gaussian Blur" ) != 0 )
    {
        return 0;
    }
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        GaussianBlur( src, dst, Size( i, i ), 0, 0 );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }

    ////////////////////////////////////中值模糊
    if( display_caption( "Median Blur" ) != 0 )
    {
        return 0;
    }
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        medianBlur ( src, dst, i );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }

    //////////////////////////////////////双边滤波
    if( display_caption( "Bilateral Blur" ) != 0 )
    {
        return 0;
    }
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        bilateralFilter ( src, dst, i, i*2, i/2 );
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }
    display_caption( "Done!" );
    return 0;
}

//设置对应的模糊文字到图片上
int display_caption( const char* caption )
{
    dst = Mat::zeros( src.size(), src.type() );
    putText( dst, caption,
             Point( src.cols/4, src.rows/2),
             FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );
    return display_dst(DELAY_CAPTION);
}

//显示并延迟delay时间
int display_dst( int delay )
{
    imshow( window_name, dst );
    int c = waitKey ( delay );
    if( c >= 0 ) { return -1; }
    return 0;
}
