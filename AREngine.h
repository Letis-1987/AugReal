#ifndef ARENGINE_H
#define ARENGINE_H

#include <opencv2/opencv.hpp>
#include <deque>
#include <memory>

#include "ARFrame.h"
#include "ARFrameProcessor.h"
#include "ARViewer.h"

using FrameQueue = std::deque<std::shared_ptr<ARFrame>>;

class AREngine
{
private:
    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    std::shared_ptr<ARFrame> origin = std::make_shared<ARFrame>(cv::Mat());
    std::vector<cv::Point3f> grid3D;

    void averageGrid(const std::vector<std::vector<cv::Point2f> > &grids, std::shared_ptr<ARFrame> &frame);
    bool PnP(std::shared_ptr<ARFrame> &frame);
    bool isSample(std::shared_ptr<ARFrame> frame);
    bool createMask(std::shared_ptr<ARFrame> frame);

public:
    AREngine() = default;
    ~AREngine();

    cv::BFMatcher matcher;
    FrameQueue sampleQueue;
    size_t maxSamples = 3;
    FrameQueue frameQueue;
    size_t maxFrames = 5;
    ARFrameProcessor* frameProcessor = new ARFrameProcessor(this);
    ARViewer* viewer = new ARViewer(this);
    cv::Mat cameraMatrix, distortionMatrix; // Camera parameter matrixes

    bool setOrigin(cv::Mat image);
    bool setNextFrame(const cv::Mat& frame); // Begin processing next frame True - if marker finded
    cv::Mat getLastFrame() const; // Get last processed frame or empty matrix (cv::Mat::empty() == true)
};

#endif // ARENGINE_H
