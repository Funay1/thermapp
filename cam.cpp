#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <pthread.h>
#include "thermapp.h"

#include <stdio.h>
using namespace std;
using namespace cv;
Mat frame,frame1;
VideoCapture cap;
char buffer1[100];
void abreCam(){
    if(!cap.open(0))
        return;
}


Mat rotate(Mat src, double angle)
{
    angle = 90;
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols-60, src.rows+80));
    return dst;
}
void *saveThread(void *)
{

    imwrite(buffer1,frame);
}
void *showThread(void *)
{
        imshow("smile :)",frame);
}

void pegaWebcam(){
    pthread_t tid;
    void *status;

    time_t rawtime;
    time(&rawtime);
    static unsigned long long int imageNumber = 0;

    sprintf (buffer1, "imagens/rgb/%s%d.png",ctime(&rawtime),imageNumber++);
    cap >> frame1;
    frame = rotate(frame1, 90);
    if( frame.empty() ){ printf("\n\n\n\nFALHOU\n\n\n\n"); return;} // end of video stream
    pthread_create(&tid, NULL, saveThread, NULL);
    pthread_join(tid, &status);
    pthread_create(&tid, NULL, showThread, NULL);
   pthread_join(tid, &status);

    //imshow("this is you, smile! :)", frame);
//    imwrite(buffer1,frame);
    //printf("\n\n\n\nAQUI\n\n\n\n");
}
