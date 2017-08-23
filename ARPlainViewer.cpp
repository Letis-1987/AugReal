#include "ARPlainViewer.h"
#include "AREngine.h"

ARPlainViewer::ARPlainViewer(AREngine *engine) : ARViewer(engine)
{

}

void ARPlainViewer::paintAugment(std::shared_ptr<ARFrame> &frame)
{
    if (engine->frameQueue.size() < 3)
    {
        return;
    }
    else if (!engine->frameQueue[1]->found && !engine->frameQueue[2]->found)
    {
        modelsQueue.clear();
    }
    std::vector<cv::Point2f> temp;
    if (*frame)
    {
        cv::projectPoints(model, frame->rvec, frame->tvec, engine->cameraMatrix, engine->distortionMatrix, temp);
        modelsQueue.push_front(temp);
        if (modelsQueue.size() > 3)
            modelsQueue.resize(3);
    }
    if (modelsQueue.size() >= 3)
    {
        if (temp.empty()) temp = modelsQueue.front();
        for (size_t i = 1; i < modelsQueue.size(); ++i)
            for (size_t j = 0; j < modelsQueue[j].size(); ++j)
                temp[j] += modelsQueue[i][j];
        for (cv::Point2f& p : temp)
            p /= 3;

        for (const std::pair<unsigned int, unsigned int>& p : indexes)
            cv::line(frame->frame, temp[p.first], temp[p.second], cv::Scalar(255, 0, 0));
    }

}

void ARPlainViewer::setModel(std::vector<cv::Point3f> model, std::vector<std::pair<unsigned int, unsigned int> > indexes)
{
    this->model = model;
    this->indexes = indexes;
}
