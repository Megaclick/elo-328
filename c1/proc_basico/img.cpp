#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[]){
    if(argc==1){
        std::cout<<"usage: ./img image"<<std::endl;
    }
    cv::Mat img;
    img = cv::imread(argv[1]);
    
    if(img.empty()){
        std::cout<<"error loading img"<<std::endl;
        return -1;
    }
    imshow("imagen",img);
    cv::waitKey(0);
    return 0;
}