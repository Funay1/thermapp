#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "thermapp.h"
#include <stdio.h>
using namespace std;
using namespace cv;
VideoCapture cap;
char buffer1[100];
void abreCam(){
    if(!cap.open(0))
        return;
}


Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

void pegaWebcam(){
    time_t rawtime;
    time(&rawtime);
    static unsigned long long int imageNumber = 0;

    sprintf (buffer1, "imagens/rgb/%s%d.png",ctime(&rawtime),imageNumber++);
    Mat frame,frame1;
    cap >> frame1;
    frame = rotate(frame1, 90);
    if( frame.empty() ){ printf("\n\n\n\nFALHOU\n\n\n\n"); return;} // end of video stream
    imshow("this is you, smile! :)", frame);
    imwrite(buffer1,frame);
    //printf("\n\n\n\nAQUI\n\n\n\n");
}
