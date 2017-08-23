#ifndef ARFRAMEPROCESSOR_H
#define ARFRAMEPROCESSOR_H

#include<opencv2/opencv.hpp>
#include<vector>
#include<memory>
#include<utility>

#include "ARFrame.h"
class AREngine;

using SetPointsPair = std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>>;

class ARFrameProcessor
{
private:
    AREngine* engine;

    SetPointsPair match(std::shared_ptr<ARFrame> frame, std::shared_ptr<ARFrame> sample); // Return "good keys"
    std::vector<cv::Point2f> createGrid(const std::vector<cv::Point2f>& frameGoodPoints, const std::vector<cv::Point2f>& sampleGrid, const std::vector<cv::Point2f>& sampleGoodPoints);
    bool checkGrid(std::vector<cv::Point2f> grid) const;

public:
    ARFrameProcessor(AREngine* engine);
    ARFrameProcessor* clone() const;
    std::vector<cv::Point2f> processing(std::shared_ptr<ARFrame> frame, std::shared_ptr<ARFrame> sample); // Return true - if marker finded

};

#endif // ARFRAMEPROCESSOR_H
