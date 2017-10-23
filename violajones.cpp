#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;

 /** Function Headers */
 unsigned long long int detectAndDisplay( Mat frame , Mat frameTermica );
 void CallBackFunc(int event, int x, int y, int flags, void* userdata);
 void CallBackFuncElipse(int event, int x, int y, int flags, void* userdata);

 /** Global variables */
 String face_cascade_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_frontalface_alt2.xml";
 String eyes_cascade_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/frontalEyes35x16.xml";
 String nose_cascade_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_mcs_nose.xml";
 String mouth_cascade_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_mcs_mouth.xml";
 String teste_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_frontalface_alt.xml";
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 CascadeClassifier nose_cascade;
 CascadeClassifier mouth_cascade;
 CascadeClassifier teste;
 string window_name = "Capture - Face detection";
 RNG rng(12345);
 Point pt[4];
 Mat rotate1(Mat src, double angle)
 {
     //angle = 90;
     Mat dst;
     Point2f pt(src.cols/2., src.rows/2.);
     Mat r = getRotationMatrix2D(pt, angle, 1.0);
     warpAffine(src, dst, r, Size(src.cols-60, src.rows+80));
     return dst;
 }
 /** @function main */
unsigned long long int achou = 0;
unsigned long long int numberFrames = 0;
 int main( int argc, const char** argv )
 {
   VideoCapture capture = VideoCapture("/home/thecakeisalai/Desktop/thermapp/ThermAppCam/build-thermapp-Desktop_Qt_5_8_0_GCC_64bit-Debug/imagens matheus/MatheusRgb.mp4");
   VideoCapture captureTermica = VideoCapture("/home/thecakeisalai/Desktop/thermapp/ThermAppCam/build-thermapp-Desktop_Qt_5_8_0_GCC_64bit-Debug/imagens matheus/MatheusTermica.mp4");
   //VideoCapture capture;

   Mat frame;
   Mat frameTermica;
   Mat frame_rotacionado;
   vector<Point2f> pts_src;
   pts_src.push_back(Point2f(420, 101));
   pts_src.push_back(Point2f(419, 189));
   pts_src.push_back(Point2f(418, 273));
   pts_src.push_back(Point2f(418, 361));
   pts_src.push_back(Point2f(421, 448));
   pts_src.push_back(Point2f(423, 533));
   pts_src.push_back(Point2f(337, 532));
   pts_src.push_back(Point2f(334, 445));
   pts_src.push_back(Point2f(334, 358));
   pts_src.push_back(Point2f(336, 275));
   pts_src.push_back(Point2f(335, 186));
   pts_src.push_back(Point2f(334, 104));
   pts_src.push_back(Point2f(252, 103));
   pts_src.push_back(Point2f(251, 188));
   pts_src.push_back(Point2f(251, 271));
   pts_src.push_back(Point2f(250, 360));
   pts_src.push_back(Point2f(251, 444));
   pts_src.push_back(Point2f(252, 531));
   pts_src.push_back(Point2f(165, 532));
   pts_src.push_back(Point2f(164, 449));
   pts_src.push_back(Point2f(165, 357));
   pts_src.push_back(Point2f(166, 275));
   pts_src.push_back(Point2f(167, 187));
   pts_src.push_back(Point2f(168, 105));

   vector<Point2f> pts_dst;
   pts_dst.push_back(Point2f(394+60,138+ 228));
   pts_dst.push_back(Point2f(404+60,138+ 312));
   pts_dst.push_back(Point2f(414+60,138+ 398));
   pts_dst.push_back(Point2f(424+60,138+ 483));
   pts_dst.push_back(Point2f(432+60,138+ 572));
   pts_dst.push_back(Point2f(437+60,138+ 646));
   pts_dst.push_back(Point2f(354+60,138+ 660));
   pts_dst.push_back(Point2f(345+60,138+ 579));
   pts_dst.push_back(Point2f(339+60,138+ 496));
   pts_dst.push_back(Point2f(330+60,138+ 405));
   pts_dst.push_back(Point2f(318+60,138+ 319));
   pts_dst.push_back(Point2f(308+60,138+ 242));
   pts_dst.push_back(Point2f(227+60,138+ 255));
   pts_dst.push_back(Point2f(236+60,138+ 333));
   pts_dst.push_back(Point2f(247+60,138+ 416));
   pts_dst.push_back(Point2f(256+60,138+ 498));
   pts_dst.push_back(Point2f(264+60,138+ 590));
   pts_dst.push_back(Point2f(274+60,138+ 659));
   pts_dst.push_back(Point2f(195+60,138+ 661));
   pts_dst.push_back(Point2f(183+60,138+ 591));
   pts_dst.push_back(Point2f(175+60,138+ 512));
   pts_dst.push_back(Point2f(163+60,138+ 426));
   pts_dst.push_back(Point2f(156+60,138+ 348));
   pts_dst.push_back(Point2f(151+60,138+ 264));

   Mat h = findHomography(pts_src, pts_dst);

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eye\n"); return -1; };
   if( !nose_cascade.load( nose_cascade_name ) ){ printf("--(!)Error loading nose\n"); return -1; };
   if( !mouth_cascade.load( mouth_cascade_name ) ){ printf("--(!)Error loading mouth\n"); return -1; };
   if( !teste.load( teste_name ) ){ printf("--(!)Error loading teste\n"); return -1; };
   //-- 2. Read the video stream
    //capture.open(0);
   Mat frameTermica1;
    //namedWindow("MyVideo");
     while(true)
     {
         //waitKey(100);
        capture >> frame;

        captureTermica >> frameTermica;

        //captureTermica >> frameTermica;
       // cout << frame.size << endl;

        //frame = imread("/home/thecakeisalai/Desktop/index.jpg");

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() ){
           numberFrames++;
        warpPerspective(frame, frame_rotacionado, h, frameTermica.size());
        frame = rotate1(frame_rotacionado,-360);
        achou = achou + detectAndDisplay( frame_rotacionado,frameTermica );
        waitKey(10);
        cout << "% de deteccao" << (double)achou/numberFrames << endl;
        cout << achou << endl;
        continue;
       }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }
       int c = waitKey(10);
       /*if( (char)c == 'c' ) { break; }
        if(!frame.empty()){
            cout << "teste 1" << endl;
            //imshow("MyVideo", frame);
        }
        else {
            cout << "AQUI";
        }*/

      }
     while(true){}
   return 0;
 }




//  ----- CODE TO ROTATE IMAGE
//  ----- AND FIND HOMOGRAPHY MATRIX

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
 VideoCapture capture = VideoCapture("/home/thecakeisalai/Desktop/thermapp/ThermAppCam/build-thermapp-Desktop_Qt_5_8_0_GCC_64bit-Debug/imagens\ matheus/MatheusRgbCortada.mp4");
 VideoCapture captureTermica = VideoCapture("/home/thecakeisalai/Desktop/thermapp/ThermAppCam/build-thermapp-Desktop_Qt_5_8_0_GCC_64bit-Debug/imagens\ matheus/MatheusTermicaCortada.mp4");
 Mat im_dst = imread("/home/thecakeisalai/Pictures/calibracoes/calibracao 30cm/1termica.png");
 Mat im_src = imread("/home/thecakeisalai/Pictures/calibracoes/calibracao 30cm/1rgb.png");
 Mat im_out;


//int main( int argc, char** argv)
//{
//    //captureTermica >> im_src;
//    //capture >> im_dst;
//    // Read source image.
//    cout << "oi" << endl;
//    cout << im_src.size << "aqui" << endl;;
//    // Four corners of the book in source image
//    vector<Point2f> pts_src;
//    pts_src.push_back(Point2f(420, 101));
//    pts_src.push_back(Point2f(419, 189));
//    pts_src.push_back(Point2f(418, 273));
//    pts_src.push_back(Point2f(418, 361));
//    pts_src.push_back(Point2f(421, 448));
//    pts_src.push_back(Point2f(423, 533));
//    pts_src.push_back(Point2f(337, 532));
//    pts_src.push_back(Point2f(334, 445));
//    pts_src.push_back(Point2f(334, 358));
//    pts_src.push_back(Point2f(336, 275));
//    pts_src.push_back(Point2f(335, 186));
//    pts_src.push_back(Point2f(334, 104));
//    pts_src.push_back(Point2f(252, 103));
//    pts_src.push_back(Point2f(251, 188));
//    pts_src.push_back(Point2f(251, 271));
//    pts_src.push_back(Point2f(250, 360));
//    pts_src.push_back(Point2f(251, 444));
//    pts_src.push_back(Point2f(252, 531));
//    pts_src.push_back(Point2f(165, 532));
//    pts_src.push_back(Point2f(164, 449));
//    pts_src.push_back(Point2f(165, 357));
//    pts_src.push_back(Point2f(166, 275));
//    pts_src.push_back(Point2f(167, 187));
//    pts_src.push_back(Point2f(168, 105));


//    // Read destination image.
//cout << im_dst.size << "aqui" << endl;;
//    // Four corners of the book in destination image.
//    vector<Point2f> pts_dst;
//    pts_dst.push_back(Point2f(394, 228));
//    pts_dst.push_back(Point2f(404, 312));
//    pts_dst.push_back(Point2f(414, 398));
//    pts_dst.push_back(Point2f(424, 483));
//    pts_dst.push_back(Point2f(432, 572));
//    pts_dst.push_back(Point2f(437, 646));
//    pts_dst.push_back(Point2f(354, 660));
//    pts_dst.push_back(Point2f(345, 579));
//    pts_dst.push_back(Point2f(339, 496));
//    pts_dst.push_back(Point2f(330, 405));
//    pts_dst.push_back(Point2f(318, 319));
//    pts_dst.push_back(Point2f(308, 242));
//    pts_dst.push_back(Point2f(227, 255));
//    pts_dst.push_back(Point2f(236, 333));
//    pts_dst.push_back(Point2f(247, 416));
//    pts_dst.push_back(Point2f(256, 498));
//    pts_dst.push_back(Point2f(264, 590));
//    pts_dst.push_back(Point2f(274, 659));
//    pts_dst.push_back(Point2f(195, 661));
//    pts_dst.push_back(Point2f(183, 591));
//    pts_dst.push_back(Point2f(175, 512));
//    pts_dst.push_back(Point2f(163, 426));
//    pts_dst.push_back(Point2f(156, 348));
//    pts_dst.push_back(Point2f(151, 264));


//    // Calculate Homography
//    Mat h = findHomography(pts_src, pts_dst);
//    cout << h << endl;
//    // Output image
//    // Warp source image to destination based on homography
//    /*while(true){
//        capture >> im_src;
//        captureTermica >> im_dst;
//        if( !im_src.empty() ){*/
//         //im_src = rotate1(im_src,90);
//            warpPerspective(im_src, im_out, h, im_dst.size());
//            //imshow("haha",im_out);
//            waitKey(50);
//          //detectAndDisplay( im_out,im_out );
//        /*}
//        else
//        { printf(" --(!) No captured frame -- Break!"); break; }
//    }*/
//    // Display images
//    Point2f teste;
////    vector<Point2f> centers; //this will be filled by the detected centers
////    Size patternsize(8,5);
////    char buffer[50];
////    int contador = 0;
////    for(int i=1;i<=20;i++){
////        sprintf(buffer,"r%d.png",i);
////        Mat im_dst = imread(buffer);
////    bool patternfound = findChessboardCorners(im_dst,patternsize,centers);
////    drawChessboardCorners(im_dst, patternsize, Mat(centers), patternfound);
////    for(int i=0;i<centers.size();i++){
////        if(i != 8 && i && 16 && i != 24)
////        cout << centers[i].x << " " << centers[i].y << endl;
////    }
////    if(patternfound == 1){
////        contador++;
////        cout << i << endl;
////        imshow(buffer, im_dst);
////    }
////    }
////    cout << contador << endl;

//    //ellipse( im_dst, Point(170,60)- Point(80,-190), Size( 2, 2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
//    Point2f src_center(im_src.cols/2.0F, im_src.rows/2.0F);
//    Mat rot_mat = getRotationMatrix2D(src_center, -90, 1.0);
//    Mat dst;
//    warpAffine(im_src, dst, rot_mat, im_src.size());
//    imshow("Source Image", im_src);
//    setMouseCallback("Source Image", CallBackFunc, NULL);
//    imshow("Destination Image", im_dst);
//    imshow("Warped Source Image", im_out);
//    setMouseCallback("Warped Source Image", CallBackFuncElipse, NULL);
//    waitKey(0);

//}
void CallBackFuncElipse(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
         ellipse( im_out, Point(x, y), Size( 2, 2), 0, 0, 360, Scalar( x, y, (x+y)%255 ), 4, 8, 0 );
         ellipse( im_dst, Point(x, y), Size( 2, 2), 0, 0, 360, Scalar( x, y, (x+y)%255 ), 4, 8, 0 );
         imshow("Destination Image", im_dst);
         imshow("Warped Source Image", im_out);
     }
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "pts_dst.push_back(Point2f(" << x << ", " << y << "));" << endl;
     }
}
/** @function detectAndDisplay */

int pegaMaior(std::vector<Rect>object){
    int indice = 0;
    if(object.size() > 1){ // pega maior face
        for(size_t i=0;i< object.size(); i++)
            if(object[i].width > object[indice].width){ indice = i;}
    }
    return indice;
}

unsigned long long int detectAndDisplay( Mat frame,Mat frameTermica )
{

  Mat ROI(frameTermica.rows, frameTermica.cols, CV_8UC1, Scalar(0,0,0));
  Mat frameTermicaB1 = frameTermica.clone();
  if(frame.empty() == true || frameTermica.empty() == true)
      return 0;
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(30, 30) );

  if(face_cascade.empty() == true){
      printf("aqui\n");
      return 0;
  }


  for( size_t i = 0; i < faces.size() && i < 1 ; i++ ) // procura em cada rosto
  {
    size_t eye_number;
    if(faces.empty()) return 0;
    i = pegaMaior(faces);
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    if(frame.rows < faces[i].height*0.5 + center.y || 0 > center.y - 0.5*faces[i].height)
        return 0;
    //ellipse( frameTermica, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    //ellipse( ROI, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), -4, 8, 0 );
    //imshow("teste",ROI);
    //return;
    cout << "aqui galera" << endl;
    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;
    //-- In each face, dete ct eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0,  Size(5,5));
    if(eyes_cascade.empty() == true) {
        imshow( "window_name", frame );
        cout << "nao achou o olho" << endl;
        return 0;
    }
    //cout << eyes.size() << endl;
    for( size_t j = 0; j < eyes.size() && j < 1; j++ )
    {
       eye_number = j;
       Point centerEye( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radiusEye = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       Point centerEyeLeft( faces[i].x + eyes[j].x + eyes[j].width*0.25 , faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       Point centerEyeRight( faces[i].x + eyes[j].x + eyes[j].width*0.75, faces[i].y + eyes[j].y + eyes[j].height*0.5 );



       ellipse( frameTermicaB1,centerEyeLeft, Size(eyes[j].width*0.4*0.33, eyes[j].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 0.5, 0);
       ellipse( frameTermicaB1,centerEyeRight, Size(eyes[j].width*0.4*0.33, eyes[j].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, LINE_AA, 0);

       ellipse( ROI,centerEyeLeft, Size(eyes[j].width*0.4*0.33, eyes[j].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 16, 0);
       ellipse( ROI,centerEyeRight, Size(eyes[j].width*0.4*0.33, eyes[j].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 16, 0);

    }

    //-- In each face, detect nose
    std::vector<Rect> nose;
    cv::Mat faceROI1 = cv::Mat(faceROI, cv::Rect(0,0,0.8*faceROI.rows,faceROI.cols));
    nose_cascade.detectMultiScale(faceROI1,nose,1.1,2,0,Size(30,30));
    int nose_y;
    if(nose_cascade.empty() == true){
        cout << "não achou o nariz" << endl;
        imshow( "window_name", frame );
    }
    size_t nose_number;
    int entrou_nose = 0;
    for( size_t j = 0; j < nose.size() && j < 1; j++ )
    {
       j = pegaMaior(nose);
       nose_number = j;
       Point centerNose( faces[i].x + nose[j].x + nose[j].width*0.5, faces[i].y + nose[j].y + nose[j].height*0.4);

       if(centerNose.y + nose[j].height*0.35> frame.rows || 0 > centerNose.y - 0.35*nose[j].height) continue;

       ellipse( ROI,centerNose, Size(nose[j].width*0.3*0.8, nose[j].height*0.35*0.8),0,0,360,Scalar( 255, 255, 0 ), -2, 16, 0);
       ellipse( frameTermicaB1,centerNose, Size(nose[j].width*0.3*0.8, nose[j].height*0.35*0.8),0,0,360,Scalar( 255, 255, 0 ), -2, 16, 0);



       nose_y = nose[j].y;
       entrou_nose = 1;
    }
    if(entrou_nose == 0) return 0;
//    std::vector<Rect> mouth;
//    mouth_cascade.detectMultiScale(faceROI,mouth,1.1,2,0,Size(30,30));
//    if(mouth_cascade.empty() == true){
//        cout << "não achou a boca " << endl;
//        imshow( "window_name", frame );
//    }
//    bool achou = true;
//    for( size_t j = 0; j <= mouth.size() && achou; j++ )
//    {

//       if(nose_y > mouth[j].y)
//           continue;
//       achou = false;
//       //cout << "aqui" << endl;
//       Point centerMouth( faces[i].x + mouth[j].x + mouth[j].width*0.5, faces[i].y + mouth[j].y + mouth[j].height*0.5 );
//      // int radiusMouth = cvRound( (mouth[j].width + mouth[j].height)*0.25 );
//       //circle( frame, centerMouth, radiusMouth, Scalar( 255, 0, 0 ), 4, 8, 0 );

//       if(centerMouth.y + mouth[j].height*0.25> frame.rows || 0 > centerMouth.y - 0.25*mouth[j].height) continue;

//       ellipse( frame,centerMouth, Size(mouth[j].width*0.4, mouth[j].height*0.25),0,0,360,Scalar( 255, 0, 0 ), 2, 16, 0);
//     }

    // in each face, detect cheek
    if(eyes.empty()) return 0;
    if(nose.empty()) return 0;
    if(faces[i].y + eyes[eye_number].y + eyes[eye_number].height*0.5 > faces[i].y + nose[nose_number].y + nose[nose_number].height*0.4  ) return 0;
    float p = abs(0.5*(0.5*eyes[eye_number].width - nose[nose_number].width)) ;
    //cout << p << endl;
    Point centerCheekR( faces[i].x + nose[nose_number].x + nose[nose_number].width +p+7.5, faces[i].y + nose[nose_number].y + nose[nose_number].height*0.5 );
    Point centerCheekL( faces[i].x + nose[nose_number].x -p-7.5, faces[i].y + nose[nose_number].y + nose[nose_number].height*0.5 );
    ellipse( ROI,centerCheekR, Size(0.8*(nose[nose_number].width*0.35), nose[nose_number].height*0.35),0,0,360,Scalar( 255, 0, 255 ), -2, 16, 0);
    ellipse( frameTermicaB1,centerCheekR, Size(0.8*(nose[nose_number].width*0.35), nose[nose_number].height*0.35),0,0,360,Scalar( 255, 0, 255 ), -2, 16, 0);


    //Point centerCheekL( faces[i].x + faces[i].width*0.45 - nose[nose_number].width*0.5, faces[i].y + nose[nose_number].y + nose[nose_number].height*0.5  );
    ellipse( ROI,centerCheekL, Size(0.8*(nose[nose_number].width*0.35), nose[nose_number].height*0.35),0,0,360,Scalar( 255, 0, 255 ), -2, 16, 0);
    ellipse( frameTermicaB1,centerCheekL, Size(0.8*(nose[nose_number].width*0.35), nose[nose_number].height*0.35),0,0,360,Scalar( 255, 0, 255 ), -2, 16, 0);



    Point forHeadLeft(faces[i].x + eyes[eye_number].x + eyes[eye_number].width*0.25, faces[i].y + 0.8*eyes[eye_number].y);
    ellipse( ROI,forHeadLeft,Size(eyes[eye_number].width*0.2, eyes[eye_number].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 16, 0);
    ellipse( frameTermicaB1,forHeadLeft,Size(eyes[eye_number].width*0.2, eyes[eye_number].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 16, 0);

    Point forHeadRight(faces[i].x + eyes[eye_number].x + eyes[eye_number].width*0.80, faces[i].y + 0.8*eyes[eye_number].y);
    ellipse( ROI,forHeadRight, Size(eyes[eye_number].width*0.2, eyes[eye_number].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 16, 0);
    ellipse( frameTermicaB1,forHeadRight, Size(eyes[eye_number].width*0.2, eyes[eye_number].height*0.25),0,0,360,Scalar( 255, 255, 255 ), -2, 16, 0);


//    int x[1];
//    x[0] = 1;
    // in each face, detect forhead
//    pt[0] = Point(faces[i].x + faces[i].width*0.5, faces[i].y);// ponto de cima no centro da face
//    pt[1] = Point(faces[i].x + faces[i].width*0.5, faces[i].y + eyes[i].y); // ponto de baixo no centro da face
//    pt[2] = Point(faces[i].x + eyes[i].x + eyes[i].width*0.5, faces[i].y); // ponto de cima na reta do centro do olho
//    pt[3] = Point(faces[i].x + faces[i].width*0.5 + eyes[i].width, faces[i].y + eyes[i].y); // ponto de baixo no centro da face
    //pt[4] = Point(faces[i].x + faces[i].width*0.5, faces[i].y);// ponto de cima no centro da face
    //fillPoly(frame,pt,4,x,Scalar(0,0,255));
    //Point centerFourheadR( faces[i].x + eyes[i].x + eyes[i].width*0.5, faces[i].y);
    //ellipse( frame,centerFourheadR, Size((faces[i].width - nose[i].width)*0.25*0.7, (faces[i].width - nose[i].width)*0.25*0.7),0,0,360,Scalar( 0, 0, 0 ), 2, 16, 0);


  }

  //-- Show what you got
//  Mat res;
//  if(!frameTermica.empty()){
//  bitwise_and(frameTermica,ROI,res);
//  imshow("teste",res);
  imshow( window_name, frame );
  imshow( "Termica",frameTermicaB1);
  if(faces.size() < 1) return 0;
   // Mat res;

    Mat res = frameTermica.clone();
        //imshow("teste",ROI);
    res.setTo( Scalar( 0, 0, 0 ), ~ROI);
    imshow("ROI",res);
    cout << ROI.size << frameTermica.size << res.size<<  endl;
  return 1;
 }
