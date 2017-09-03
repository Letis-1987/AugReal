#include <Qt3DQuickExtras/Qt3DQuickWindow>
#include <QGuiApplication>
#include <Qt3DQuick/QQmlAspectEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <Qt3DExtras>
#include <QQuickView>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtWidgets>

#include <iostream>
#include <opencv2/opencv.hpp>

#include "AREngine.h"
#include "ARViewer.h"
#include "ARPlainViewer.h"
#include "MySurface.h"

#define STT double s, e, f = cv::getTickFrequency(); s = cv::getTickCount();
#define ENT e = cv::getTickCount(); std::cout<<"Time: "<<(e-s) / f<<" sec"<<std::endl;

int main(int argc, char** argv)
{
//    ***** 3 *****
//   QApplication app(argc, argv);
//   QCamera cam;
//   QVideoWidget wgt;
//   QVideoProbe probe;
//   MySurface sur;
//   QObject::connect(&probe, SIGNAL(videoFrameProbed(QVideoFrame)), &sur, SLOT(frameProcessor(QVideoFrame)));
//   probe.setSource(&cam);
//   cam.setViewfinder(&wgt);
//   wgt.show();
//   cam.start();
//   wgt.resize(500, 500);
//   return app.exec();

//    ***** 2 *****
    QGuiApplication app(argc, argv);

    QQuickView view;
    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    view.setSource(QUrl("../AugReal/main.qml"));
    view.resize(700, 700);
    view.show();

    return app.exec();


//    ***** 1 *****
//    cv::Mat img;
//// Video - photo ***
//    img = cv::imread("../AugReal/Samples/photo.jpg");

//    cv::VideoCapture cap("../AugReal/Samples/video.avi");
//// Camera ***
////    cv::VideoCapture cap(cv::CAP_ANY);
////    while(cv::waitKey(33) != 32)
////    {
////        cap >> img;
////        cv::imshow("AugReal", img);
////    }
//// ***

//    AREngine eng;
//    eng.viewer->paintGridFlag = true;
//    eng.setOrigin(img);
//    cv::FileStorage fs;
//    cv::Mat cam, dist;
//    fs.open("../AugReal/Samples/CameraMatrix.xml", cv::FileStorage::READ);
//    fs["CamMat"]>>cam;
//    fs.release();
//    fs.open("../AugReal/Samples/DistortionMatrix.xml", cv::FileStorage::READ);
//    fs["DistMat"]>>dist;
//    fs.release();

//    eng.cameraMatrix = cam;
//    eng.distortionMatrix = dist;

//    double start, end, frequency = cv::getTickFrequency();

////    cv::Mat orig = img.clone();
////    cv::imshow("Original", orig);
//    delete eng.viewer;

//    ARPlainViewer* v = new ARPlainViewer(&eng);

//    int cubeEdge = img.rows / 2;
//    cv::Point3f startPoint((img.cols - cubeEdge) / 2, (img.rows - cubeEdge) / 2, 0);
//    v->setModel({startPoint,
//                {startPoint.x + cubeEdge, startPoint.y, 0},
//                {startPoint.x + cubeEdge, startPoint.y + cubeEdge, 0},
//                {startPoint.x, startPoint.y + cubeEdge, 0},
//                {startPoint.x, startPoint.y, startPoint.z - cubeEdge},
//                {startPoint.x + cubeEdge, startPoint.y, startPoint.z - cubeEdge},
//                {startPoint.x + cubeEdge, startPoint.y + cubeEdge, startPoint.z - cubeEdge},
//                {startPoint.x, startPoint.y + cubeEdge, startPoint.z - cubeEdge}},
//                {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}});
//    eng.viewer = v;

//    while(1)
//    {
//        cap >> img;
//        if (img.empty()) break;
//        std::cout<<"Start frame"<<std::endl;
//        start = cv::getTickCount();
//        eng.setNextFrame(img);
//        end = cv::getTickCount();
//        std::cout<<"End frame Time: "<<(end - start) / frequency<<" sec"<<std::endl;
//        cv::drawKeypoints(img, eng.frameQueue.front()->keyPoints, img);
//        cv::imshow("AugReal", eng.getLastFrame());

//        char c =cv::waitKey(33);
//        if (c == 32)
//        {
//            cv::waitKey();
//        }
//        else if (c != -1)
//        {
//            break;
//        }
//    }

//    return 0;

}
