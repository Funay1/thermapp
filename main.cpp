#include "mainwindow.h"
#include <QApplication>
#include "cam.h"

int main(int argc, char *argv[])
{
    abreCam();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
using namespace cv;
//int main(int argc, char** argv)
//{
//    VideoCapture cap;
//    // open the default camera, use something different from 0 otherwise;
//    // Check VideoCapture documentation.
//    if(!cap.open(0))
//        return 0;
//    for(;;)
//    {
//          Mat frame;
//          cap >> frame;
//          if( frame.empty() ) break; // end of video stream
//          imshow("this is you, smile! :)", frame);
//          if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC
//    }
//    // the camera will be closed automatically upon exit
//    // cap.close();
//    return 0;
//}

