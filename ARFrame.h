#ifndef ARFRAME_H
#define ARFRAME_H

#include <opencv2/opencv.hpp>
#include <vector>


struct ARFrame
{
    ARFrame() = default;
    ARFrame(const cv::Mat& frame);
    operator cv::Mat() const;
    operator bool() const;
    void clear();

    cv::Mat frame;
    cv::Mat mask;
    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat descriptors;
    std::vector<cv::Point2f> grid; // Support grid on the marker in the frame
    cv::Mat rvec, tvec; // Rotate and translate grid
    bool found = false; // Flag. True - if marker find in the frame
};

#endif // ARFRAME_H
