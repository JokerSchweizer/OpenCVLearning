//
// Created by welsh on 2022/4/3.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
void videoEdgesDetect(){
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) return ;

    cv::Mat frame ,edges;
    cv::namedWindow("edges",cv::WINDOW_AUTOSIZE);
    while (true){
        cap>>frame;
        cv::cvtColor(frame,edges,cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(edges,edges,cv::Size(7,7),1.5,1.5);
        cv::Canny(edges,edges,0,30,3);
        cv::imshow("edges",edges);
        if (cv::waitKey(30)>=0) break;
    }
    return;
}