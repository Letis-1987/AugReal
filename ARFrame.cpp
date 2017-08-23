#include "ARFrame.h"

ARFrame::ARFrame(const cv::Mat& frame)
{
    this->frame = frame;
}

void ARFrame::clear()
{
    frame.release();
    mask.release();
    keyPoints.clear();
    descriptors.release();
    grid.clear();
    rvec.release();
    tvec.release();
    found = false;
}

ARFrame::operator bool() const
{
    return found;
}

ARFrame::operator cv::Mat() const
{
    return frame;
}
