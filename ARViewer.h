#ifndef ARVIEWER_H
#define ARVIEWER_H

#include<opencv2/opencv.hpp>
#include<memory>

#include"ARFrame.h"
class AREngine;

class ARViewer
{
private:
    void paintGrid(std::shared_ptr<ARFrame>& frame);

protected:
    AREngine* engine;
    virtual void paintAugment(std::shared_ptr<ARFrame>& frame);

public:
    ARViewer(AREngine* engine);
    virtual ~ARViewer() = default;

    bool paintGridFlag = false;
    virtual void paint(std::shared_ptr<ARFrame> &frame);
};

#endif // ARVIEWER_H
