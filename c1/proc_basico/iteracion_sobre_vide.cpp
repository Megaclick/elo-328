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

        int depth = frame.depth(),channels = frame.channels(),step = frame.step;
        uchar *data = frame.data;

        //Forma 1 solo funciona con mat unsigned char
        /*int i, j, cols = frame.cols,rows = frame.rows;
        for(i = 0; i < rows; i++) {
            for(j = 0; j < cols; j++) {
                data[i*step + j*channels + 0]= data[i*step + j*channels + 1] = 0;
            }
        }*/


        //Forma 2, cualquier tipo de canal, pero solo funciona
        //para un canal
        /*
        cv::Mat M = frame;
        int i, j, cols = M.cols,rows = M.rows;
        for(i = 0; i < rows; i++) {
            for(j = 0; j < cols; j++) {
                cv::Vec3b pixel = M.at<unsigned char>(i,j);
                std::cout << "(i,j): " << i << "," << j << ")" << std::endl;
                std::cout << "B: "<< (int)pixel.val[0]<< "; G: " << (int)pixel.val[1]<< "; R: " << (int)pixel.val[2] << std::endl;
            }
        }
        */
       
        //Forma 3, acceso a nivel de referencia a cada pixel
        //ESTA ES LA FORMA IZI
        cv::Mat_<cv::Vec3b> _I = frame;
        for(int i = 0; i < frame.rows; ++i){
            for(int j = 0; j < frame.cols; ++j) {
                _I(i,j)[0] = 0;
                //_I(i,j)[1] = _I(i,j)[1];
                _I(i,j)[2] = 0;
            }
        }
        frame = _I;
        
        
        //cv::imshow("Forma1", frame);
        //cv::imshow("Forma2", M);
        cv::imshow("Forma3", frame);
        
        if(cv::waitKey(50) >= 0) break;
    }
    
    return 0;
}