#include "opencv2/opencv.hpp"
#include<iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
cv::Mat applyGamma(cv::Mat image,int x, int y, int w, int h, bool flag,std::map<int, float> table_value){
    cv::Mat_<cv::Vec3b> _I = image;  //Separo los canales, en _I[0] se encuentra luma
    cv::Mat img;
    int x1,x2,y1,y2;
    x1 = x;
    x2 = x+w;
    y1 = y;
    y2 = y+h;
    if(flag){
        for (int i = 0; i < image.rows; i++)
        {
            for (int j = 0; j < image.cols; j++)
            {
                if(i > x1 && j > y1 && i < x2 && j < y2 ){
                    _I(i,j)[0]= table_value[(int)_I(i,j)[0]]; 
                }
            }
        }   
    }
    else{
        for (int i = 0; i < image.rows; i++)
        {
            for (int j = 0; j < image.cols; j++)
            {
                _I(i,j)[0]= table_value[(int)_I(i,j)[0]]; 
            }
        }
    }
    img = _I;
    return img;
}
cv::Mat applyFrame(cv::Mat image,int x, int y, int w, int h,int r,int g,int b){
    cv::Mat_<cv::Vec3b> _I = image;  //Separo los canales
    int x1,x2,y1,y2;
    x1 = x;
    x2 = x+w;
    y1 = y;
    y2 = y+h;   
    for (int i = 0; i < image.rows; i++)
        {
        for (int j = 0; j < image.cols; j++)
            {
            if(i <= x1 || j <= y1 || i >= x2 || j >= y2){
                _I(i,j)[0]=b; 
                _I(i,j)[1]=g;
                _I(i,j)[2]=r;
            }
            }
        }
    return _I;
}
int main(int argc, char *argv[]) {
    // Args
    float GAMMA;
    int x,y,w,h,r,g,b;
    bool flag = false;
    r = 0;
    g = 0;
    b = 0;
    for(int i=0; i<argc; ++i)
    {  
        if (std::string(argv[i])=="-f")
        {
            x = atoi(argv[i+1]);
            y = atoi(argv[i+2]);
            w = atoi(argv[i+3]);
            h = atoi(argv[i+4]);
            flag = true;
        } 
        if (std::string(argv[i])=="-c"){
            r = atoi(argv[i+1]);
            g = atoi(argv[i+2]);
            b = atoi(argv[i+3]);
        }     
    }
    //-------------

    //Mats usados por las funciones
    float pix_value;
    cv::Mat img;
    cv::Mat dst;
    cv::Mat ycrcb;
    cv::Mat color_frame;
    cv::Mat out;
    //--------------

    //Video o imagen
    if(std::string(argv[1]) == "-v"){
        GAMMA = atof(argv[2]);
        if(GAMMA<=0){
            std::cout << "Invalid Gamma, must be greater than 0" << std::endl;
            return 0;
        }   
        std::map<int, float> table_value;
        for (int i = 0; i < 256; i++)
        {
            pix_value = i/255.0;
            table_value[i] = pow (pix_value,GAMMA)*255;
        }
        cv::VideoCapture vid;
        vid.open(0); 
        if(!vid.isOpened()) {
            cerr << "Error opening input." << endl;
            return 1; 
        }

        while(1) {
            vid >> img;
            img.copyTo(dst);
            cv::cvtColor(dst, ycrcb, cv::COLOR_BGR2YCrCb);
            dst = applyGamma(ycrcb,x,y,w,h,flag,table_value);
            cv::cvtColor(dst, out, cv::COLOR_YCrCb2BGR);
            cv::cvtColor(dst, out, cv::COLOR_YCrCb2BGR);        
            color_frame = applyFrame(out,x,y,w,h,r,g,b);
            imshow("Original", img);
            imshow("Gamma", color_frame);

            if(cv::waitKey(10) != -1)
                break;
        }
        vid.release();
        return 0;

    }
    else if(std::string(argv[1]) == "-i"){
        GAMMA = atof(argv[3]);
        std::map<int, float> table_value;
        for (int i = 0; i < 256; i++)
        {
            pix_value = i/255.0;
            table_value[i] = pow (pix_value,GAMMA)*255;
        }
        if(GAMMA<=0)
        {
            std::cout << "Invalid Gamma, must be greater than 0" << std::endl;
            return 0;
        }   
        img = cv::imread(std::string(argv[2]),CV_LOAD_IMAGE_COLOR);
        if(!img.data)                         
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }

        img.copyTo(dst);
        cv::cvtColor(dst, ycrcb, cv::COLOR_BGR2YCrCb);
        dst = applyGamma(ycrcb,x,y,w,h,flag,table_value);
        cv::cvtColor(dst, out, cv::COLOR_YCrCb2BGR);        
        color_frame = applyFrame(out,x,y,w,h,r,g,b);
        imshow("Original", img);
        imshow("Gamma", color_frame); 
        cv::waitKey(0); 
        return 0;
    }
    
    else{
        std::cout << "invalid input." << std::endl;
        std::cout << "./GAMMA -v gamma [-f x y w h] [-c r g b]\n"
        "./GAMMA -i imagen gamma [-f x y w h] [-c r g b]\n" 
        "-v: usar webcam.\n"
        "-i: usar imagen.\n"
        "image: Ruta de imagen a procesar.\n"
        "gamma: Nivel de Gamma. Debe ser un valor real, mayor que 0.\n"
        "-f: (Optional) Define a rea a la que aplica gamma. Resto es borde.\n"
        "Se define rectangulo posicion (x,y) y tamanno W x H.\n"
        "-c:    (Optional) Color (r,g,b) del frame." << std::endl;
        return 0;
    }


    return 0;
}