
/**

Point 二维点表示结构
      Point pt =  Point(10, 8);
      Point pt;
      pt.x = 10;
      pt.y = 8;


Scalar 表示一个 4 元素向量。Scalar 类型在 OpenCV 中广泛用于传递像素值
      Scalar( a, b, c )
      通常用这个来定义BGR颜色，Blue=a,Green=b,Red=c;


*/


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define w 400
using namespace cv;
void MyEllipse( Mat img, double angle ); //画一个角度的椭圆
void MyFilledCircle( Mat img, Point center ); //画一个圆，给定圆心
void MyPolygon( Mat img );
void MyLine( Mat img, Point start, Point end );
int Draw( void ){
  //创建两个400*400白布
  char atom_window[] = "Drawing 1: Atom";
  char rook_window[] = "Drawing 2: Rook";
  Mat atom_image = Mat::zeros( w, w, CV_8UC3 );
  Mat rook_image = Mat::zeros( w, w, CV_8UC3 );
  //画了四个椭圆，按照不同角度摆放
  MyEllipse( atom_image, 90 );
  MyEllipse( atom_image, 0 );
  MyEllipse( atom_image, 45 );
  MyEllipse( atom_image, -45 );
  //画一个圆
  MyFilledCircle( atom_image, Point( w/2, w/2) );


  MyPolygon( rook_image );
  rectangle( rook_image,
         Point( 0, 7*w/8 ),
         Point( w, w),
         Scalar( 0, 255, 255 ),
         FILLED,
         LINE_8 );
  MyLine( rook_image, Point( 0, 15*w/16 ), Point( w, 15*w/16 ) );
  MyLine( rook_image, Point( w/4, 7*w/8 ), Point( w/4, w ) );
  MyLine( rook_image, Point( w/2, 7*w/8 ), Point( w/2, w ) );
  MyLine( rook_image, Point( 3*w/4, 7*w/8 ), Point( 3*w/4, w ) );
  imshow( atom_window, atom_image );
  moveWindow( atom_window, 0, 200 );
  imshow( rook_window, rook_image );
  moveWindow( rook_window, w, 200 );
  waitKey( 0 );
  return(0);
}


//画椭圆
void MyEllipse( Mat img, double angle )
{
  int thickness = 2;  //线条宽度
  int lineType = 8;   //线条类型，这个可以设置反锯齿等 LINE_4 LINE_8 LINE_AA
  ellipse( img,       //被画的图像
       Point( w/2, w/2 ),  //椭圆圆心
       Size( w/4, w/16 ),  //长短轴
       angle,           //旋转角度
       0,               //开始角度
       360,             //结束角度
       Scalar( 255, 0, 0 ),   //颜色
       thickness,         //线条粗细
       lineType );        //线条类型
}

//画圆
void MyFilledCircle( Mat img, Point center )
{
  circle( img,
      center,
      w/32,
      Scalar( 0, 0, 255 ),
      FILLED,
      LINE_8 );
}

void MyPolygon( Mat img )
{
  int lineType = LINE_8;
  Point rook_points[1][20];
  rook_points[0][0]  = Point(    w/4,   7*w/8 );
  rook_points[0][1]  = Point(  3*w/4,   7*w/8 );
  rook_points[0][2]  = Point(  3*w/4,  13*w/16 );
  rook_points[0][3]  = Point( 11*w/16, 13*w/16 );
  rook_points[0][4]  = Point( 19*w/32,  3*w/8 );
  rook_points[0][5]  = Point(  3*w/4,   3*w/8 );
  rook_points[0][6]  = Point(  3*w/4,     w/8 );
  rook_points[0][7]  = Point( 26*w/40,    w/8 );
  rook_points[0][8]  = Point( 26*w/40,    w/4 );
  rook_points[0][9]  = Point( 22*w/40,    w/4 );
  rook_points[0][10] = Point( 22*w/40,    w/8 );
  rook_points[0][11] = Point( 18*w/40,    w/8 );
  rook_points[0][12] = Point( 18*w/40,    w/4 );
  rook_points[0][13] = Point( 14*w/40,    w/4 );
  rook_points[0][14] = Point( 14*w/40,    w/8 );
  rook_points[0][15] = Point(    w/4,     w/8 );
  rook_points[0][16] = Point(    w/4,   3*w/8 );
  rook_points[0][17] = Point( 13*w/32,  3*w/8 );
  rook_points[0][18] = Point(  5*w/16, 13*w/16 );
  rook_points[0][19] = Point(    w/4,  13*w/16 );

  const Point* ppt[1] = { rook_points[0] };
  int npt[] = { 20 };
  //画多边形
  fillPoly( img,
        ppt,
        npt,
        1,
        Scalar( 255, 255, 255 ),
        lineType );
}

void MyLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = LINE_8;
  //画线
  line( img,
    start,
    end,
    Scalar( 0, 0, 0 ),
    thickness,
    lineType );
}
