#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtDebug"
#include <QtCore>
#include <unistd.h>
#include <ctmf.h>
#include <ctime>
#include <iostream>
#include <stdio.h>
using namespace cv;
char buffer [100];
bool saveImages = false;
bool calibracao = false;
bool aux1 = true;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void insertionSort(int window[],int tam)
{
    int temp, i , j;
    for(i = 0; i < tam; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

void median(short* const src,short* const dst){
    int window[9];

    for(int y = 1; y < 288 - 1; y++){
        for(int x = 1; x < 384- 1; x++){

            // Pick up window element

            window[0] = src[(y-1)*384 + (x-1)];
            window[1] = src[(y-0)*384 + (x-1)];
            window[2] = src[(y+1)*384 + (x-1)];
            window[3] = src[(y-1)*384 + (x-0)];
            window[4] = src[(y-0)*384 + (x-0)];
            window[5] = src[(y+1)*384 + (x-0)];
            window[6] = src[(y-1)*384 + (x+1)];
            window[7] = src[(y-0)*384 + (x+1)];
            window[8] = src[(y+1)*384 + (x+1)];

            // sort the window to find median
            insertionSort(window,9);

            // assign the median to centered element of the matrix
            dst[y*384 + x] = window[4];
        }
    }
}


void MainWindow::getMinMax(pair *minmax, signed short *arr, int n)
{
  float mid;
  int i;
  int teste = 0;

  //If there is only one element then return it as min and max both
  if (n == 1)
  {
     minmax->max = arr[0];
     minmax->min = arr[0];
     return;
  }

  //If there are more than one elements, then initialize min
  //    and max

  if (arr[0] > arr[1])
  {
      minmax->max = arr[0];
      minmax->min = arr[1];

  }
  else
  {
      minmax->max = arr[1];
      minmax->min = arr[0];
  }

  mid = arr[0] + arr[1];

  for (i = 2; i<n; i++)
  {
      if(arr[i] < 180){
          teste++;
          continue;}
      if(arr[i] > 350){
          continue;
          teste++;
      }
      mid += arr[i];
    if (arr[i] >  minmax->max)
      minmax->max = arr[i];

    else if (arr[i] <  minmax->min)
      minmax->min = arr[i];
  }
  //fprintf(stderr, "teste = %d\n",teste);
  //sleep(2);
  minmax->mid = mid/n;
  //minmax->min = 280;
  //minmax->max = 330;

    return;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Download calibrate data 25 deg C
    FILE *fp= fopen("ruido.bin", "rb");
    if(fp != NULL){
       printf("aqu");
      fread(image_cal, 2, 384*288, fp);
      fclose(fp);
    }

    therm = thermapp_initUSB();
    if(therm == NULL) {
        qDebug()<< "init Error";
    }

    /// Debug -> check for thermapp
    if(thermapp_USB_checkForDevice(therm, VENDOR, PRODUCT) == -1){
       qDebug()<< "USB_checkForDevice Error";
    }else
    {
        qDebug()<< "thermapp_Request_thread";
        //Run thread usb therm
        thermapp_FrameRequest_thread(therm);
    }

    /// Run frame refresh timer
    frameTimer = new QTimer(this);
    connect(frameTimer, SIGNAL(timeout()), this, SLOT(UpdateProcessFrame()));
    frameTimer->start(100);
    //thermapp_setGain(therm,0);


    // wfp = fopen("gain0.txt", "w");
    temp_prev = thermapp_getTemperature(therm);

    count = 0;

    //on_GainSlider_valueChanged(gain_kof[0]);

    //sTimer = new QTimer(this);
    //connect(sTimer, SIGNAL(timeout()), this, SLOT(sample()));
    //sTimer->start(600);


    ui->VoutA_Slider->setValue(therm->cfg->VoutA);
    ui->VoutC_Slider->setValue(therm->cfg->VoutC);
    ui->VoutD_Slider->setValue(therm->cfg->VoutD);
    ui->VoutE_Slider->setValue(therm->cfg->VoutE);


    ui->L_VoutA->setText(QString("VoutA: %1").arg(((float)2.5/2048) * therm->cfg->VoutA));
    ui->L_VoutC->setText(QString("VoutC: %1").arg(((float)2.5/2048) * therm->cfg->VoutC));
    ui->L_VoutD->setText(QString("VoutD: %1").arg(((float)2.5/2048) * therm->cfg->VoutD));
    ui->L_VoutE->setText(QString("VoutE: %1").arg(((float)2.5/2048) * therm->cfg->VoutE));

    gainCal = 1.27;
    ui->gainLabel->setText(QString("%1").arg(1));
    offsetCal = 130;
    ui->offsetLabel->setText(QString("%1").arg(0));

    ui->spinBox->setValue(2);
}

MainWindow::~MainWindow()
{
    thermapp_Close(therm);
    delete ui;
}


///FIXME: This method transfers RAW data from ThermApp to QPixmap
QPixmap MainWindow::Frame(short *frame){

   int pix_lim, i, tmp = 0, tmp0 = 0;
   int i_p=0;
   int teste = 0;


  short frame_trans[PIXELS_DATA_SIZE];

   for(i = 0; i < PIXELS_DATA_SIZE; i++){
       frame_trans[i] = ((frame[i] - image_cal[i]) * gainCal) + offsetCal;
   }
   FILE *pFile = fopen ("myfile.txt","w");
   for(i=0;i<PIXELS_DATA_SIZE;i++){
       fprintf (pFile, "%d ",frame[i]/10);
         frame[i] = frame[i]/10;
   }
   fclose(pFile);

//  median(frame1,frame);
  getMinMax(&min_max, frame, PIXELS_DATA_SIZE);

  //fprintf(stderr, "min = %d, mid = %d max = %d\n",min_max.min,min_max.mid,min_max.max);

  //ctmf(frame,frame,384,288,1,1,3,1,512*1024);
  ui->labelDinamic->setText(QString("Dynamic range: %1").arg(sqrt((min_max.max - min_max.min) * (min_max.max - min_max.min))));
  ui->minmaxlabel->setText(QString("max: %1, min: %2").arg(min_max.max).arg(min_max.min));

  // Mostrar Imagem de acordo com a imagem base  //

    //inverte_matriz(frame_trans);
    for(i = 0; i < PIXELS_DATA_SIZE; i++){

        pix_lim = frame_trans[i];
        if((pix_lim > 255) && (tmp == 0)){
            tmp = 1;
            qDebug() << "overflow > " << i << pix_lim;
            //int agc = ui->gainCalSlider->value();
            //agc -= 50;
            //ui->gainCalSlider->setValue(agc);

        }

        if((pix_lim < 0) && (tmp0 == 0)){
            tmp0 = 1;
            //int agc = ui->offsetCalSlider->value();
            qDebug() << "overflow < "<< i << pix_lim;
            //agc += sqrt(min_max.min * min_max.min);
            //ui->offsetCalSlider->setValue(agc);
        }

        if(pix_lim > 255)
            pix_lim = 255;

        if(pix_lim < 0){
            //qDebug() << "overflow <";
            pix_lim = 0;
        }
        //
        //printf("%d tmp = %d\n",pix_lim,tmp);
        imageData[i_p] = (unsigned char)pix_lim;
        i_p++;
        imageData[i_p] = (unsigned char)pix_lim;
        i_p++;
        imageData[i_p] = (unsigned char)pix_lim;
        i_p++;

    }
    for(int j=250;j<700;j= j + 3){
       for(int i=220;i<240;i= i + 3){
                imageData[384*i+0-390+j + 384*612] = (700-j)/2;
                imageData[384*i+1-390+j+ 384*612] = (700-j)/2;
                imageData[384*i+2-390+j+ 384*612] = (700-j)/2;
        }
    }

    QPixmap pixmap = QPixmap::fromImage(
        QImage(
            (unsigned char *) imageData,
            384,
            288,
            QImage::Format_RGB888
                   // ,QImage::Format_Mono
        )
    );
    return pixmap.transformed(QTransform().rotate(-90).scale(-2, 2));
}


void saveThermalImage(QPixmap pixmap){
    static unsigned long long int imageNumber = 0;
    time_t rawtime;
    time(&rawtime);
    //sprintf (buffer, "imagens/%s%d.png","img",imageNumber++);
    sprintf (buffer, "imagens/termica/%s%d.png",ctime(&rawtime),imageNumber++);
    QFile file(buffer);
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file,"PNG");
    file.close();
}


void MainWindow::UpdateProcessFrame(){
    //while(1);
    QPixmap aux;
    QPixmap aux2;
    aux2.load("scala.png");
    //qDebug()<<  "in ----->>> UpdateProcessFrame()";
    short frame[PIXELS_DATA_SIZE];



    if(thermapp_GetImage(therm, frame))
    {
        aux = Frame(frame);
        QPainter painter( &aux );
        painter.setFont( QFont("Arial") );
        QPen penHLines(QColor("#0e5a77"), 10, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
        painter.setPen(penHLines);
        painter.drawText( QPoint(540, 150), QString::number(min_max.max/10) );
        painter.drawText( QPoint(553, 150), "ºC" );
        painter.drawText( QPoint(540, 480), QString::number(min_max.min/10) );
        painter.drawText( QPoint(553, 480), "ºC" );
        if(saveImages){
          /*  FILE *fp= fopen("ruido.bin", "wb");
            fwrite(image_cal , sizeof(short), sizeof(image_cal), fp);
            fclose(fp);*/
            pegaWebcam();
            //popen("adb shell am broadcast -a take_picture","r);
            //system("adb shell am broadcast -a take_picture");
            saveThermalImage(aux);
            //saveThermalTemperature();
            /*namedWindow("MyVideo");
            Mat img = imread(buffer);
            Mat imgX, imgY,imgFinal;

            GaussianBlur(img,img,Size(11,11),0,0);
            cvtColor( img, img, CV_BGR2GRAY );
            Sobel(img,imgY,CV_16S,0,1);
            Sobel(img,imgX,CV_16S,1,0);
            convertScaleAbs( imgX, imgX );
            convertScaleAbs( imgY, imgY );
            addWeighted( imgX, 0.5, imgY, 0.5, 0, imgFinal );
            img = imgFinal+img;

            //fastNlMeansDenoising(img,img,30,7,21);
             imshow("MyVideo",img);*/
        }
        //cvtColor(img, img, CV_RGB2GRAY);
        //fastNlMeansDenoisingMulti(img,img,3.0,7,21);
        //qDebug() << "temp: " << thermapp_getTemperature(therm) << " pixel: " << pixel0 <<" radiation: "<< ui->lineEdit->text();
        ui->scen->setPixmap(aux);
        ui->scaleBar->setPixmap(aux2);
//        ui->scaleBar->setIconSize(QSize(360,360));
        ui->id->setText(QString("%1").arg(thermapp_getId(therm)));
        ui->frames->setText(QString("%1").arg(thermapp_getFrameCount(therm)));
        // qDebug()<<  thermapp_getFrameCount(therm);
        //printf("%d\n", thermapp_getFrameCount(therm));
        ui->temp->setText(QString("%1").arg(thermapp_getTemperature(therm)));
        ui->error_frames->setText(QString("%1").arg(therm->lost_packet));
    }
   // qDebug()<<  "out ----->>> UpdateProcessFrame()";

}


void MainWindow::sample(){

}

// Get Calibrate data
void MainWindow::on_getCalibButton_pressed()
{
    calibracao = true;
    frameTimer->stop();
    short frame[PIXELS_DATA_SIZE];
    double d_frame[PIXELS_DATA_SIZE];

    while(!thermapp_GetImage(therm, frame));

    for(int i = 0; i < PIXELS_DATA_SIZE; i++){
        d_frame[i] = frame[i];
        //fprintf(stderr, " %f",(frame[i] - 14336) * 0.00652);
    }

    for(int i = 0; i < 50; i++){
        while(!thermapp_GetImage(therm, frame));

        for(int j = 0; j < PIXELS_DATA_SIZE; j++){
            d_frame[j] += frame[j];
        }
    }

    for(int i = 0; i < PIXELS_DATA_SIZE; i++){
         image_cal[i] = d_frame[i] / 50;
    }

    frameTimer->start(100);


}

void MainWindow::on_setCalibButton_pressed()
{

}



void MainWindow::on_VoutA_Slider_valueChanged(int value)
{
    ui->L_VoutA->setText(QString("VoutA: %1").arg(((float)2.5/2048) * value));
    therm->cfg->VoutA = value;
}

void MainWindow::on_VoutC_Slider_valueChanged(int value)
{
    ui->L_VoutC->setText(QString("VoutC: %1").arg(((float)2.5/2048) * value));
    therm->cfg->VoutC = value;
}

void MainWindow::on_VoutD_Slider_valueChanged(int value)
{
    ui->L_VoutD->setText(QString("VoutD: %1").arg(((float)2.5/2048) * value));
    therm->cfg->VoutD = value;
}

void MainWindow::on_VoutE_Slider_valueChanged(int value)
{
    ui->L_VoutE->setText(QString("VoutE: %1").arg(((float)2.5/2048) * value));
    therm->cfg->VoutE = value;
}


/// Debug  some test
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
#if 0
    int temp = (value<<16)&0xffff0000;
#else
    int temp = (value)&0x0000ffff;
#endif
  //  therm->cfg->none_volatile_data5 = temp;
    ui->label_3->setText(QString("%1").arg(temp));
}

/// Debug
void MainWindow::on_gainSlider_valueChanged(int value)
{
    gainCal = (double)value/32768;
    ui->gainLabel->setText(QString("%1").arg(gainCal));

}

void MainWindow::on_offsetSlider_valueChanged(int value)
{
    offsetCal = value;
    ui->offsetLabel->setText(QString("%1").arg(offsetCal));
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    therm->cfg->modes = arg1 & 0x000f;
}

void MainWindow::on_clearButton_pressed()
{
    memset(image_cal, 0, sizeof(image_cal));
}

void MainWindow::on_pushButton_clicked(bool checked)
{
        saveImages = true;
        ui->pushButton->setText("Gravando");
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    saveImages = false;
    ui->pushButton->setText("Gravar");
}

void MainWindow::on_getCalibButton_clicked()
{

}
