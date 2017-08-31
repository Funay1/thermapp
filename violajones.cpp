#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;

 /** Function Headers */
 void detectAndDisplay( Mat frame , Mat frameTermica );
 void CallBackFunc(int event, int x, int y, int flags, void* userdata);
 void CallBackFuncElipse(int event, int x, int y, int flags, void* userdata);

 /** Global variables */
 String face_cascade_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_frontalface_alt.xml";
 String eyes_cascade_name = "/home/thecakeisalai/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
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
 /** @function main */
/*
 int main( int argc, const char** argv )
 {
   VideoCapture capture = VideoCapture("out.mp4");
   VideoCapture captureTermica = VideoCapture("termicaout.mp4");
   //VideoCapture capture;
   Mat frame;
   Mat frameTermica;
   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eye\n"); return -1; };
   if( !nose_cascade.load( nose_cascade_name ) ){ printf("--(!)Error loading nose\n"); return -1; };
   if( !mouth_cascade.load( mouth_cascade_name ) ){ printf("--(!)Error loading mouth\n"); return -1; };
   if( !teste.load( teste_name ) ){ printf("--(!)Error loading teste\n"); return -1; };
   //-- 2. Read the video stream
    //capture.open(0);
    namedWindow("MyVideo");
     while(true)
     {
         waitKey(100);
        capture >> frame;
        captureTermica >> frameTermica;
   //     imshow("antes",frame);
   //     frame = imread("/home/thecakeisalai/Desktop/index.jpg");

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
        detectAndDisplay( frame,frameTermica );
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
        if(!frame.empty()){
            cout << "teste 1" << endl;
            imshow("MyVideo", frame);
        }
        else {
            cout << "AQUI";
        }
      }
   return 0;
 }
*/
/*
 int main (){
     //namedWindow("MyVideo");
     Mat rotMatrix;
     Mat rotMatrixRgb;
     Mat rot_vec = Mat::zeros(1,3,CV_32F);
     rot_vec.at<float>(0,0) = 0.0195;
     rot_vec.at<float>(0,1) = 0.0371;
     rot_vec.at<float>(0,2) = 0.0235;
     //Rodrigues(rot_vec,rot_matrix,teste); // matrix de rotacao
     Mat T(3,1,cv::DataType<float>::type); // vetor de translacao
     T.at<float>(0,0) = 30.1545;
     T.at<float>(1,0) = -22.4858;
     T.at<float>(2,0) = 121.8940;
     cv::Mat intrinsicParameter(3,3,cv::DataType<float>::type); // intrinsic parameter matrix
     intrinsicParameter.at<float>(0,0) = 772.5578;
     intrinsicParameter.at<float>(0,1) = 0;
     intrinsicParameter.at<float>(0,2) = 238.2199;
     intrinsicParameter.at<float>(1,0) = 0;
     intrinsicParameter.at<float>(1,1) = 775.7107;
     intrinsicParameter.at<float>(1,2) = 83.0067;
     intrinsicParameter.at<float>(2,0) = 0;
     intrinsicParameter.at<float>(2,1) = 0;
     intrinsicParameter.at<float>(2,2) = 1;

640,480

     cv::Mat distCoeffs(4,1,cv::DataType<float>::type); // coeficientes de distorcao -- CONSIDERANDO SEM DISTORCAO
     distCoeffs.at<float>(0) = 0;
     distCoeffs.at<float>(1) = 0;
     distCoeffs.at<float>(2) = 0;
     distCoeffs.at<float>(3) = 0;


     //projectPoints(objectPoints, rvecR, T, K, distCoeffs, projectedPoints);
     Mat frametermica = imread("termica.png");
     Mat framergb = imread("rgb.png");
     Point RgbPoint(80+150,120);

     cv::Mat RgbPointMatrix(3,1,cv::DataType<float>::type);
     RgbPointMatrix.at<float>(0,0) = 150+80-211;
     RgbPointMatrix.at<float>(1,0) = 120+240-150;
     RgbPointMatrix.at<float>(2,0) = 0;


     cv::Mat Crgb(3,1,cv::DataType<float>::type);
     Crgb.at<float>(0,0) = 211+80+30;
     Crgb.at<float>(1,0) = 150;
     Crgb.at<float>(2,0) = 0;

     cv::Mat translacaoCameraRgb(3,1,cv::DataType<float>::type);
    translacaoCameraRgb.at<float>(0,0) = -16.356461;
    translacaoCameraRgb.at<float>(1,0) = 34.683937;
    translacaoCameraRgb.at<float>(2,0) = 214.805847;

    cv::Mat rotacaoCameraRgb(1,3,cv::DataType<float>::type);
    rotacaoCameraRgb.at<float>(0,0) = -0.080300;
    rotacaoCameraRgb.at<float>(0,1) = -0.178932;
    rotacaoCameraRgb.at<float>(0,2) = -1.136063;
    Rodrigues(rotacaoCameraRgb,rotMatrixRgb);


     float x = RgbPoint.x -211.5966;
     float y = RgbPoint.y - 150.4523;
     float z = 870; //870;
     cv::Mat pontoMundoRgb(3,1,cv::DataType<float>::type);
    pontoMundoRgb.at<float>(0,0) = x;
    pontoMundoRgb.at<float>(1,0) = y;
    pontoMundoRgb.at<float>(2,0) = z;

    Mat pRgb = rotMatrixRgb*RgbPointMatrix + translacaoCameraRgb;

    cv::Mat rotacaoCamera(1,3,cv::DataType<float>::type);
   rotacaoCamera.at<float>(0,0) = 0.0195;
   rotacaoCamera.at<float>(0,1) = 0.0371;
   rotacaoCamera.at<float>(0,2) = 0.0235;
   Rodrigues(rotacaoCamera,rotMatrix);


   cv::Mat translacaoCamera(3,1,cv::DataType<float>::type);
  translacaoCamera.at<float>(0,0) = 30.1545;
  translacaoCamera.at<float>(1,0) = -22.4858;
  translacaoCamera.at<float>(2,0) = 121.8940;


    std::vector<cv::Point2f> projectedPoints;
    std::vector<cv::Point2d> imagePoints;
    vector<cv::Point3f> mundoTermica;
    Mat pTermica = rotMatrix*pRgb + translacaoCamera;
    //Point3d projectedPoints;


    //cout << "aqui" << pTermica.at<float>(0,0);
    Point3f xyzTermica(pTermica.at<float>(0,0),pTermica.at<float>(1,0),pTermica.at<float>(2,0));
    cout << "aqu testei" << xyzTermica.z<< endl;
    mundoTermica.push_back(xyzTermica);
    projectPoints(mundoTermica, rot_vec, T, intrinsicParameter, distCoeffs, projectedPoints);

    cout << pRgb.at<float>(0,0) << endl;
    cout << pontoMundoRgb.at<float>(0,0) << endl;
    cout << pRgb.at<float>(1,0) << endl;
    cout << pontoMundoRgb.at<float>(1,0) << endl;
    cout << pRgb.at<float>(2,0) << endl;
    cout << pontoMundoRgb.at<float>(2,0) << endl;


    cout << projectedPoints[0].x << endl;
    cout << projectedPoints[0].y << endl;

    projectedPoints[0].y = fabs(projectedPoints[0].y);
    projectedPoints[0].x = fabs(projectedPoints[0].x);


    cout << "frame rgb" <<framergb.size << endl;
    cout << "frame termica" << frametermica.size << endl;


    ellipse( framergb, RgbPoint, Size( 2, 2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    ellipse( frametermica, projectedPoints[0], Size( 2, 2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    while(waitKey(27)){
        /*cout << framergb.size << endl;
        cout << frametermica.size << endl;
        imshow("RGB",framergb);
        imshow("TERMICA",frametermica);
    }
    //cout << pTermica.size;
    cout << "aqui";
     return 0;
 }
*/


#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
 Mat im_dst = imread("termica.png");
 Mat im_src = imread("rgb.png");
 Mat im_out;


int main( int argc, char** argv)
{
    // Read source image.
    cout << "oi" << endl;
    cout << im_src.size << "aqui" << endl;;
    // Four corners of the book in source image
    vector<Point2f> pts_src;
    pts_src.push_back(Point2f(220, 248));
    pts_src.push_back(Point2f(218, 285));
    pts_src.push_back(Point2f(214, 323));
    pts_src.push_back(Point2f(211, 361));
    pts_src.push_back(Point2f(207, 399));
    pts_src.push_back(Point2f(479, 267));
    pts_src.push_back(Point2f(477, 304));
    pts_src.push_back(Point2f(474, 343));
    pts_src.push_back(Point2f(473, 380));
    pts_src.push_back(Point2f(472, 417));
    pts_src.push_back(Point2f(334, 258));
    pts_src.push_back(Point2f(330, 293));
    pts_src.push_back(Point2f(328, 331));
    pts_src.push_back(Point2f(326, 371));
    pts_src.push_back(Point2f(323, 408));
    pts_src.push_back(Point2f(225, 195));
    pts_src.push_back(Point2f(222, 225));
    pts_src.push_back(Point2f(220, 255));
    pts_src.push_back(Point2f(217, 289));
    pts_src.push_back(Point2f(214, 323));
    pts_src.push_back(Point2f(454, 214));
    pts_src.push_back(Point2f(457, 244));
    pts_src.push_back(Point2f(460, 275));
    pts_src.push_back(Point2f(460, 307));
    pts_src.push_back(Point2f(464, 342));
    pts_src.push_back(Point2f(324, 203));
    pts_src.push_back(Point2f(324, 233));
    pts_src.push_back(Point2f(324, 266));
    pts_src.push_back(Point2f(323, 298));
    pts_src.push_back(Point2f(323, 332));
    pts_src.push_back(Point2f(221, 268));
    pts_src.push_back(Point2f(218, 307));
    pts_src.push_back(Point2f(213, 343));
    pts_src.push_back(Point2f(209, 378));
    pts_src.push_back(Point2f(207, 414));
    pts_src.push_back(Point2f(505, 290));
    pts_src.push_back(Point2f(495, 325));
    pts_src.push_back(Point2f(488, 359));
    pts_src.push_back(Point2f(481, 394));
    pts_src.push_back(Point2f(474, 428));
    pts_src.push_back(Point2f(346, 277));
    pts_src.push_back(Point2f(340, 315));
    pts_src.push_back(Point2f(334, 351));
    pts_src.push_back(Point2f(330, 386));
    pts_src.push_back(Point2f(324, 421));
    pts_src.push_back(Point2f(221, 248));
    pts_src.push_back(Point2f(217, 284));
    pts_src.push_back(Point2f(214, 324));
    pts_src.push_back(Point2f(211, 362));
    pts_src.push_back(Point2f(206, 401));
    pts_src.push_back(Point2f(478, 269));
    pts_src.push_back(Point2f(476, 304));
    pts_src.push_back(Point2f(474, 341));
    pts_src.push_back(Point2f(471, 379));
    pts_src.push_back(Point2f(470, 417));
    pts_src.push_back(Point2f(333, 257));
    pts_src.push_back(Point2f(330, 295));
    pts_src.push_back(Point2f(327, 333));
    pts_src.push_back(Point2f(325, 370));
    pts_src.push_back(Point2f(322, 408));
    pts_src.push_back(Point2f(145, 234));
    pts_src.push_back(Point2f(156, 271));
    pts_src.push_back(Point2f(163, 309));
    pts_src.push_back(Point2f(172, 345));
    pts_src.push_back(Point2f(181, 382));
    pts_src.push_back(Point2f(401, 172));
    pts_src.push_back(Point2f(410, 208));
    pts_src.push_back(Point2f(420, 244));
    pts_src.push_back(Point2f(430, 281));
    pts_src.push_back(Point2f(440, 315));
    pts_src.push_back(Point2f(258, 208));
    pts_src.push_back(Point2f(268, 242));
    pts_src.push_back(Point2f(276, 280));
    pts_src.push_back(Point2f(284, 317));
    pts_src.push_back(Point2f(295, 355));
    pts_src.push_back(Point2f(251, 187));
    pts_src.push_back(Point2f(239, 224));
    pts_src.push_back(Point2f(226, 259));
    pts_src.push_back(Point2f(215, 296));
    pts_src.push_back(Point2f(202, 333));
    pts_src.push_back(Point2f(500, 267));
    pts_src.push_back(Point2f(490, 302));
    pts_src.push_back(Point2f(479, 335));
    pts_src.push_back(Point2f(469, 373));
    pts_src.push_back(Point2f(459, 411));
    pts_src.push_back(Point2f(359, 222));
    pts_src.push_back(Point2f(348, 255));
    pts_src.push_back(Point2f(337, 292));
    pts_src.push_back(Point2f(325, 328));
    pts_src.push_back(Point2f(314, 368));


    // Read destination image.
cout << im_dst.size << "aqui" << endl;;
    // Four corners of the book in destination image.
    vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(98, 435));
    pts_dst.push_back(Point2f(97, 472));
    pts_dst.push_back(Point2f(95, 508));
    pts_dst.push_back(Point2f(93, 546));
    pts_dst.push_back(Point2f(93, 583));
    pts_dst.push_back(Point2f(352, 448));
    pts_dst.push_back(Point2f(351, 484));
    pts_dst.push_back(Point2f(350, 519));
    pts_dst.push_back(Point2f(350, 555));
    pts_dst.push_back(Point2f(347, 593));
    pts_dst.push_back(Point2f(211, 443));
    pts_dst.push_back(Point2f(209, 480));
    pts_dst.push_back(Point2f(206, 515));
    pts_dst.push_back(Point2f(206, 553));
    pts_dst.push_back(Point2f(205, 588));
    pts_dst.push_back(Point2f(102, 384));
    pts_dst.push_back(Point2f(102, 411));
    pts_dst.push_back(Point2f(98, 444));
    pts_dst.push_back(Point2f(97, 477));
    pts_dst.push_back(Point2f(95, 508));
    pts_dst.push_back(Point2f(331, 398));
    pts_dst.push_back(Point2f(332, 424));
    pts_dst.push_back(Point2f(336, 455));
    pts_dst.push_back(Point2f(336, 487));
    pts_dst.push_back(Point2f(341, 518));
    pts_dst.push_back(Point2f(202, 391));
    pts_dst.push_back(Point2f(202, 418));
    pts_dst.push_back(Point2f(201, 447));
    pts_dst.push_back(Point2f(201, 479));
    pts_dst.push_back(Point2f(203, 515));
    pts_dst.push_back(Point2f(96, 456));
    pts_dst.push_back(Point2f(94, 491));
    pts_dst.push_back(Point2f(94, 528));
    pts_dst.push_back(Point2f(92, 565));
    pts_dst.push_back(Point2f(90, 597));
    pts_dst.push_back(Point2f(376, 469));
    pts_dst.push_back(Point2f(370, 502));
    pts_dst.push_back(Point2f(363, 538));
    pts_dst.push_back(Point2f(356, 568));
    pts_dst.push_back(Point2f(349, 601));
    pts_dst.push_back(Point2f(220, 461));
    pts_dst.push_back(Point2f(217, 497));
    pts_dst.push_back(Point2f(213, 535));
    pts_dst.push_back(Point2f(209, 567));
    pts_dst.push_back(Point2f(206, 597));
    pts_dst.push_back(Point2f(98, 435));
    pts_dst.push_back(Point2f(95, 473));
    pts_dst.push_back(Point2f(94, 509));
    pts_dst.push_back(Point2f(92, 548));
    pts_dst.push_back(Point2f(90, 583));
    pts_dst.push_back(Point2f(352, 449));
    pts_dst.push_back(Point2f(351, 484));
    pts_dst.push_back(Point2f(350, 521));
    pts_dst.push_back(Point2f(349, 556));
    pts_dst.push_back(Point2f(347, 590));
    pts_dst.push_back(Point2f(210, 441));
    pts_dst.push_back(Point2f(206, 476));
    pts_dst.push_back(Point2f(205, 516));
    pts_dst.push_back(Point2f(205, 553));
    pts_dst.push_back(Point2f(202, 589));
    pts_dst.push_back(Point2f(28, 424));
    pts_dst.push_back(Point2f(37, 459));
    pts_dst.push_back(Point2f(49, 495));
    pts_dst.push_back(Point2f(58, 532));
    pts_dst.push_back(Point2f(68, 566));
    pts_dst.push_back(Point2f(277, 355));
    pts_dst.push_back(Point2f(288, 392));
    pts_dst.push_back(Point2f(296, 428));
    pts_dst.push_back(Point2f(307, 461));
    pts_dst.push_back(Point2f(318, 497));
    pts_dst.push_back(Point2f(136, 393));
    pts_dst.push_back(Point2f(145, 433));
    pts_dst.push_back(Point2f(155, 465));
    pts_dst.push_back(Point2f(164, 503));
    pts_dst.push_back(Point2f(174, 539));
    pts_dst.push_back(Point2f(130, 371));
    pts_dst.push_back(Point2f(119, 405));
    pts_dst.push_back(Point2f(107, 442));
    pts_dst.push_back(Point2f(95, 480));
    pts_dst.push_back(Point2f(85, 514));
    pts_dst.push_back(Point2f(377, 447));
    pts_dst.push_back(Point2f(365, 480));
    pts_dst.push_back(Point2f(358, 515));
    pts_dst.push_back(Point2f(347, 552));
    pts_dst.push_back(Point2f(336, 584));
    pts_dst.push_back(Point2f(237, 403));
    pts_dst.push_back(Point2f(227, 437));
    pts_dst.push_back(Point2f(216, 475));
    pts_dst.push_back(Point2f(205, 510));
    pts_dst.push_back(Point2f(195, 547));


    // Calculate Homography
    Mat h = findHomography(pts_src, pts_dst);

    // Output image
    // Warp source image to destination based on homography
    warpPerspective(im_src, im_out, h, im_dst.size());
    // Display images
    Point2f teste;
//    vector<Point2f> centers; //this will be filled by the detected centers
//    Size patternsize(8,5);
//    char buffer[50];
//    int contador = 0;
//    for(int i=1;i<=20;i++){
//        sprintf(buffer,"r%d.png",i);
//        Mat im_dst = imread(buffer);
//    bool patternfound = findChessboardCorners(im_dst,patternsize,centers);
//    drawChessboardCorners(im_dst, patternsize, Mat(centers), patternfound);
//    for(int i=0;i<centers.size();i++){
//        if(i != 8 && i && 16 && i != 24)
//        cout << centers[i].x << " " << centers[i].y << endl;
//    }
//    if(patternfound == 1){
//        contador++;
//        cout << i << endl;
//        imshow(buffer, im_dst);
//    }
//    }
//    cout << contador << endl;

    //ellipse( im_dst, Point(170,60)- Point(80,-190), Size( 2, 2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    imshow("Source Image", im_src);
    setMouseCallback("Source Image", CallBackFunc, NULL);
    imshow("Destination Image", im_dst);
    imshow("Warped Source Image", im_out);
    setMouseCallback("Warped Source Image", CallBackFuncElipse, NULL);
    waitKey(0);

}
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
void detectAndDisplay( Mat frame,Mat frameTermica )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(30, 30) );
  if(face_cascade.empty() == true){
      printf("aqui");
      return;
  }

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    if(frame.rows < faces[i].height*0.5 + center.y || 0 > center.y - 0.5*faces[i].height)
        continue;
    ellipse( frameTermica, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    imshow( window_name, frameTermica );
    return;
    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 , Size(30, 30) );

    for( size_t j = 0; j < eyes.size() && j < 2; j++ )
     {
        printf("\n%d\n",eyes.size());
       Point centerEye( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radiusEye = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       ellipse( frame,centerEye, Size(eyes[j].width*0.4, eyes[j].height*0.25),0,0,360,Scalar( 255, 255, 255 ), 2, 16, 0);
     }

    //-- In each face, detect nose
    std::vector<Rect> nose;
    nose_cascade.detectMultiScale(faceROI,nose,1.1,2,0,Size(30,30));
    int nose_y;
    for( size_t j = 0; j < nose.size(); j++ )
     {
       Point centerNose( faces[i].x + nose[j].x + nose[j].width*0.5, faces[i].y + nose[j].y + nose[j].height*0.4 );

       if(centerNose.y + nose[j].height*0.35> frame.rows || 0 > centerNose.y - 0.35*nose[j].height) continue;

       ellipse( frame,centerNose, Size(nose[j].width*0.35, nose[j].height*0.35),0,0,360,Scalar( 255, 255, 0 ), 2, 16, 0);
       nose_y = nose[j].y;
     }
    return;
    std::vector<Rect> mouth;
    mouth_cascade.detectMultiScale(faceROI,mouth,1.1,2,0,Size(30,30));
    for( size_t j = 0; j <= mouth.size(); j++ )
     {
       if(nose_y > mouth[j].y)
           continue;
       Point centerMouth( faces[i].x + mouth[j].x + mouth[j].width*0.5, faces[i].y + mouth[j].y + mouth[j].height*0.5 );
      // int radiusMouth = cvRound( (mouth[j].width + mouth[j].height)*0.25 );
       //circle( frame, centerMouth, radiusMouth, Scalar( 255, 0, 0 ), 4, 8, 0 );

       if(centerMouth.y + mouth[j].height*0.25> frame.rows || 0 > centerMouth.y - 0.25*mouth[j].height)

       ellipse( frame,centerMouth, Size(mouth[j].width*0.4, mouth[j].height*0.25),0,0,360,Scalar( 255, 0, 0 ), 2, 16, 0);
     }

    // in each face, detect cheek
    /*Point centerCheekR( faces[i].x + faces[i].width*0.75, faces[i].y + nose[i].y + nose[i].height*0.5  );
    ellipse( frame,centerCheekR, Size((faces[i].width - nose[i].width)*0.25*0.7, (faces[i].width - nose[i].width)*0.25*0.7),0,0,360,Scalar( 0, 0, 0 ), 2, 16, 0);
    Point centerCheekL( faces[i].x + faces[i].width*0.25, faces[i].y + nose[i].y + nose[i].height*0.5  );
    ellipse( frame,centerCheekL, Size((faces[i].width - nose[i].width)*0.25*0.7, (faces[i].width - nose[i].width)*0.25*0.7),0,0,360,Scalar( 0, 0, 0 ), 2, 16, 0);
*/
    int x[1];
    x[0] = 1;
    // in each face, detect forhead
    pt[0] = Point(faces[i].x + faces[i].width*0.5, faces[i].y);// ponto de cima no centro da face
    pt[1] = Point(faces[i].x + faces[i].width*0.5, faces[i].y + eyes[i].y); // ponto de baixo no centro da face
    pt[2] = Point(faces[i].x + eyes[i].x + eyes[i].width*0.5, faces[i].y); // ponto de cima na reta do centro do olho
    pt[3] = Point(faces[i].x + faces[i].width*0.5 + eyes[i].width, faces[i].y + eyes[i].y); // ponto de baixo no centro da face
    //pt[4] = Point(faces[i].x + faces[i].width*0.5, faces[i].y);// ponto de cima no centro da face
    //fillPoly(frame,pt,4,x,Scalar(0,0,255));
    //Point centerFourheadR( faces[i].x + eyes[i].x + eyes[i].width*0.5, faces[i].y);
    //ellipse( frame,centerFourheadR, Size((faces[i].width - nose[i].width)*0.25*0.7, (faces[i].width - nose[i].width)*0.25*0.7),0,0,360,Scalar( 0, 0, 0 ), 2, 16, 0);


  }
  //-- Show what you got
  imshow( window_name, frame );
 }
