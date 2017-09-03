#ifndef MYSURFACE_H
#define MYSURFACE_H

#include <QtMultimedia>
#include <opencv2/opencv.hpp>

class MySurface : public QObject
{
    Q_OBJECT
private:
    cv::Mat img;
    bool present(const QVideoFrame &frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;

public:
    MySurface();

public slots:
    void frameProcessor(QVideoFrame frame);

};

#endif // MYSURFACE_H
