#include "MySurface.h"

#include <iostream>

bool MySurface::present(const QVideoFrame &frame)
{
    std::cout<<"qwqeqe"<<std::endl;
    return true;
}

QList<QVideoFrame::PixelFormat> MySurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB565;
}

MySurface::MySurface()
{

}

void MySurface::frameProcessor(QVideoFrame frame)
{
    frame.map(QAbstractVideoBuffer::ReadOnly);
    std::cout<<(unsigned long)frame.bits()<<std::endl;
    cv::Mat src(frame.height() + frame.height() / 2, frame.width(), CV_8UC1, frame.bits());
    cv::Mat dist(frame.height(), frame.width(), CV_8UC3);
    cv::cvtColor(src, dist, CV_YUV2BGR_YV12);
    img = dist;
    frame.unmap();
    cv::imshow("OpenCV Window", img);
}
