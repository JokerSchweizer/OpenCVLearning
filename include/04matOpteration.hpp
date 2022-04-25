/**

==============================输入输出======================================

 Mat img-imread("imag.jpg");
 Mat img = imread（文件名，IMREAD_GRAYSCALE）；
imwrite("imag.jpg",img)

使用cv::imdecode和cv::imencode从/向内存而不是文件读取和写入图像。

==============================访问像素强度值======================================
为了获得像素强度值，您必须知道图像的类型和通道数,
 Scalar intensity = img.at<uchar>(y, x); //注意 x 和 y 的顺序
 Scalar intensity = img.at<uchar>(Point(x, y));


现在让我们考虑一个具有 BGR 颜色排序的 3 通道图像
       Vec3b intensity = img.at<Vec3b>(y, x);
       uchar blue = intensity.val[0];
       uchar green = intensity.val[1];
       uchar red = intensity.val[2];

       Vec3f intensity = img.at<Vec3f>(y, x);
        float blue = intensity.val[0];
        float green = intensity.val[1];
        float red = intensity.val[2];

改变像素强度
      img.at<uchar>(y, x) = 128;

from calib3d module, such as cv::projectPoints,matrix can be easily constructed from std::vector

          vector<Point2f> points;
           //... fill the array
           Mat pointsMat = Mat(points);
          // access a point in this matrix
          Point2f point = pointsMat.at<Point2f>(i, 0);




==============================内存管理和引用计数======================================
Mat 是一种保持矩阵/图像特征（行数和列数、数据类型等）和指向数据的指针的结构。因此，没有什么能阻止我们拥有多个 Mat 实例对应相同的数据。Mat 保留一个引用计数，该计数告诉当 Mat 的特定实例被销毁时是否必须释放数据。

        std::vector<Point3f> points;
        // .. fill the array
        Mat pointsMat = Mat(points).reshape(1);

As a result, we get a 32FC1 matrix with 3 columns instead of 32FC3 matrix with 1 column.
.pointsMat uses data from points and will not deallocate the memory when destroyed.
In this particular instance, however, developer has to make sure that lifetime of points is longer than of pointsMat
If we need to copy the data, this is done using, for example, cv::Mat::copyTo or cv::Mat::clone:

      Mat img = imread("image.jpg");
      Mat img1 = img.clone();


==============================原始操作======================================
灰度图像制作黑色图像
      img = Scalar(0);

选择感兴趣的区域
      Rect r(10, 10, 100, 100);
      Mat smallImg = img(r);

从彩色到灰度的转换
      Mat img = imread("image.jpg"); // loading a 8UC3 image
      Mat grey;
      cvtColor(img, grey, COLOR_BGR2GRAY);


将图像类型从 8UC1 更改为 32FC1
      src.convertTo(dst, CV_32F);

==============================可视化图像======================================

对waitKey()的调用启动了一个消息传递循环，该循环等待“图像”窗口中的击键。一张 32F 的图像需要转换成 8U 类型

    Mat img = imread("image.jpg");
    Mat grey;
    cvtColor(img, grey, COLOR_BGR2GRAY);
    Mat sobelx;
    Sobel(grey, sobelx, CV_32F, 1, 0);
    double minVal, maxVal;
    minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
    Mat draw;
    sobelx.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", draw);
    waitKey();







这里cv::namedWindow不是必需的，因为它紧随其后的是cv::imshow。不过，它可用于更改窗口属性或使用cv::createTrackbar
*/
