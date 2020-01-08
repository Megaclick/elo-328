#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture cap(0);

    if(!cap.isOpened())
        return -1;

    cv::namedWindow("webcam");

    while(1)
    {
        cv::Mat frame;

        cap >> frame; 
        cv::imshow("webcam", frame);

        if(cv::waitKey(50) >= 0) break;
    }
    
    return 0;
}