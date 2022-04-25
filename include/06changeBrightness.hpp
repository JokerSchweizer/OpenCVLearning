/**

g( x ) = α f( x ) + β

参数α > 0和b通常称为增益和偏置参数；分别控制对比度和亮度。偏差会提高亮度，但同时图像会出现轻微的面纱.

g(i,j)=α⋅f(i,j)+β
i和j表示该像素位于第i行第j列。


Gamma 校正可用于通过使用输入值和映射输出值之间的非线性变换来校正图像的亮度：
O=pow(I/255,γ)×255

由于这种关系是非线性的，因此所有像素的效果都不相同，并且取决于它们的原始值

伽玛校正代码
  Mat lookUpTable(1, 256, CV_8U);
  uchar* p = lookUpTable.ptr();
  for( int i = 0; i < 256; ++i)
      p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);

  Mat res = img.clone();
  LUT(img, lookUpTable, res);
*/



#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
// we're NOT "using namespace std;" here, to avoid collisions between the beta variable and std::beta in c++17
using std::cin;
using std::cout;
using std::endl;
using namespace cv;
int main( int argc, char** argv )
{
  //We load an image using cv::imread and save it in a Mat object
    CommandLineParser parser( argc, argv, "{@input | lena.jpg | input image}" );
    Mat image = imread( samples::findFile( parser.get<String>( "@input" ) ) );
    if( image.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }
    //nitial pixel values equal to zero, Same size and type as the original image
    Mat new_image = Mat::zeros( image.size(), image.type() );

    //We ask now the values of α and β to be entered by the user
    double alpha = 1.0; /*< Simple contrast control */
    int beta = 0;       /*< Simple brightness control */
    cout << " Basic Linear Transforms " << endl;
    cout << "-------------------------" << endl;
    cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
    cout << "* Enter the beta value [0-100]: ";    cin >> beta;

    //我们可以简单地使用以下命令，而不是使用for循环来访问每个像素
    //image.convertTo(new_image, -1, alpha, beta);

    //perform the operation g(i,j)=α⋅f(i,j)+β we will access to each pixel in image
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*image.at<Vec3b>(y,x)[c] + beta );
            }
        }
    }

    //show the images
    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey();
    return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
https://github.com/opencv/opencv/blob/4.x/samples/cpp/tutorial_code/ImgProc/changing_contrast_brightness_image/changing_contrast_brightness_image.cpp
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

// we're NOT "using namespace std;" here, to avoid collisions between the beta variable and std::beta in c++17
using std::cout;
using std::endl;
using namespace cv;

namespace
{
/** Global Variables */
int alpha = 100;
int beta = 100;
int gamma_cor = 100;
Mat img_original, img_corrected, img_gamma_corrected;

void basicLinearTransform(const Mat &img, const double alpha_, const int beta_)
{
    Mat res;
    img.convertTo(res, -1, alpha_, beta_);

    hconcat(img, res, img_corrected);
    imshow("Brightness and contrast adjustments", img_corrected);
}

void gammaCorrection(const Mat &img, const double gamma_)
{
    CV_Assert(gamma_ >= 0);
    //! [changing-contrast-brightness-gamma-correction]
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);

    Mat res = img.clone();
    LUT(img, lookUpTable, res);
    //! [changing-contrast-brightness-gamma-correction]

    hconcat(img, res, img_gamma_corrected);
    imshow("Gamma correction", img_gamma_corrected);
}

void on_linear_transform_alpha_trackbar(int, void *)
{
    double alpha_value = alpha / 100.0;
    int beta_value = beta - 100;
    basicLinearTransform(img_original, alpha_value, beta_value);
}

void on_linear_transform_beta_trackbar(int, void *)
{
    double alpha_value = alpha / 100.0;
    int beta_value = beta - 100;
    basicLinearTransform(img_original, alpha_value, beta_value);
}

void on_gamma_correction_trackbar(int, void *)
{
    double gamma_value = gamma_cor / 100.0;
    gammaCorrection(img_original, gamma_value);
}
}

int main( int argc, char** argv )
{
    CommandLineParser parser( argc, argv, "{@input | lena.jpg | input image}" );
    img_original = imread( samples::findFile( parser.get<String>( "@input" ) ) );
    if( img_original.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }

    img_corrected = Mat(img_original.rows, img_original.cols*2, img_original.type());
    img_gamma_corrected = Mat(img_original.rows, img_original.cols*2, img_original.type());

    hconcat(img_original, img_original, img_corrected);
    hconcat(img_original, img_original, img_gamma_corrected);

    namedWindow("Brightness and contrast adjustments");
    namedWindow("Gamma correction");

    createTrackbar("Alpha gain (contrast)", "Brightness and contrast adjustments", &alpha, 500, on_linear_transform_alpha_trackbar);
    createTrackbar("Beta bias (brightness)", "Brightness and contrast adjustments", &beta, 200, on_linear_transform_beta_trackbar);
    createTrackbar("Gamma correction", "Gamma correction", &gamma_cor, 200, on_gamma_correction_trackbar);

    on_linear_transform_alpha_trackbar(0, 0);
    on_gamma_correction_trackbar(0, 0);

    waitKey();

    imwrite("linear_transform_correction.png", img_corrected);
    imwrite("gamma_correction.png", img_gamma_corrected);

    return 0;
}
