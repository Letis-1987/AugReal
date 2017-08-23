#ifndef ARPlainViewer_H
#define ARPlainViewer_H

#include <deque>

#include "ARViewer.h"
class AREngine;

class ARPlainViewer : public ARViewer
{
public:
    ARPlainViewer(AREngine* engine);
    virtual ~ARPlainViewer() = default;

    std::vector<cv::Point3f> model;
    std::vector<std::pair<unsigned int, unsigned int>> indexes;
    std::deque<std::vector<cv::Point2f>> modelsQueue;

protected:
    virtual void paintAugment(std::shared_ptr<ARFrame>& frame);

public:
    void setModel(std::vector<cv::Point3f> model, std::vector<std::pair<unsigned int, unsigned int>> indexes);
};

#endif // ARPlainViewer_H
