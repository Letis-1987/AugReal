#include "ARViewer.h"

void ARViewer::paintGrid(std::shared_ptr<ARFrame> &frame)
{
    cv::line(frame->frame, frame->grid[0], frame->grid[1], cv::Scalar(0, 255, 0));
    cv::line(frame->frame, frame->grid[1], frame->grid[2], cv::Scalar(0, 255, 0));
    cv::line(frame->frame, frame->grid[2], frame->grid[3], cv::Scalar(0, 255, 0));
    cv::line(frame->frame, frame->grid[3], frame->grid[0], cv::Scalar(0, 255, 0));

    for (const cv::Point2f& p : frame->grid)
        cv::line(frame->frame, p, p, cv::Scalar(0, 255, 0));
}

void ARViewer::paintAugment(std::shared_ptr<ARFrame> &frame)
{

}

ARViewer::ARViewer(AREngine *engine)
{
    this->engine = engine;
}

void ARViewer::paint(std::shared_ptr<ARFrame>& frame)
{
    if (paintGridFlag)
        paintGrid(frame);

    this->paintAugment(frame);
}
