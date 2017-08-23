#include "AREngine.h"
#include <thread>
#include <future>
#include <functional>

void AREngine::averageGrid(const std::vector<std::vector<cv::Point2f> >& grids, std::shared_ptr<ARFrame>& frame)
{
    if (grids.empty()) return;

    if (grids.size() == 1)
    {
        frame->grid = grids[0];
        return;
    }

    frame->grid = grids[0];
    for (size_t i = 1; i < grids.size(); ++i)
        for (size_t j = 0; j < grids[i].size(); ++j)
        {
            frame->grid[j] += grids[i][j];
        }
    for (size_t i = 0; i < frame->grid.size(); ++i)
        frame->grid[i] /= static_cast<float>(grids.size());
    return;
}

bool AREngine::PnP(std::shared_ptr<ARFrame>& frame)
{
    return cv::solvePnP(grid3D, frame->grid, cameraMatrix, distortionMatrix, frame->rvec, frame->tvec );
}

bool AREngine::isSample(std::shared_ptr<ARFrame> frame)
{
    if (*frame)
    {
        sampleQueue.push_front(frame);
        createMask(frame);
        detector->detectAndCompute(frame->frame, frame->mask, frame->keyPoints, frame->descriptors);
        if (sampleQueue.size() > maxSamples)
            sampleQueue.resize(maxSamples);
        return true;
    }
    return false;
}

bool AREngine::createMask(std::shared_ptr<ARFrame> frame)
{
    if (frame->grid.empty()) return false;
    frame->mask = cv::Mat::zeros(frame->frame.rows, frame->frame.cols, CV_8U);

    std::pair<float, float> minmaxX = std::minmax({frame->grid[0].x, frame->grid[1].x, frame->grid[1].x, frame->grid[3].x});
    std::pair<float, float> minmaxY = std::minmax({frame->grid[0].y, frame->grid[1].y, frame->grid[1].y, frame->grid[3].y});

    int top     = minmaxY.first < 0 ? 0 : minmaxY.first;
    int bottom  = minmaxY.second >= frame->frame.rows ? frame->frame.rows - 1 : minmaxY.second;
    int left    = minmaxX.first < 0 ? 0 : minmaxX.first;
    int right   = minmaxX.second >= frame->frame.cols ? frame->frame.cols - 1 : minmaxX.second;

    frame->mask(cv::Rect2f(left, top, right - left, bottom - top)) = 255;
    return true;
}

AREngine::~AREngine()
{
    delete frameProcessor;
    delete viewer;
}

bool AREngine::setOrigin(cv::Mat image)
{
    origin->clear();
    if (image.empty()) return false;
    origin->frame = image.clone();
    origin->found = true;
    detector->detectAndCompute(origin->frame, cv::Mat(), origin->keyPoints, origin->descriptors);

    // Grid generation
    std::vector<cv::Point2f> grid;
    grid.push_back(cv::Point2f(0, 0));
    grid.push_back(cv::Point2f(origin->frame.cols - 1, 0));
    grid.push_back(cv::Point2f(origin->frame.cols - 1, origin->frame.rows - 1));
    grid.push_back(cv::Point2f(0, origin->frame.rows - 1));
    for (int i = 0; i < origin->frame.rows; i += origin->frame.rows / 10)
        for (int j = 0; j < origin->frame.cols; j += origin->frame.cols/ 10)
            grid.push_back(cv::Point2f(i, j));
    origin->grid = std::move(grid);

    // 3D grid generation
    grid3D.clear();
    for (cv::Point2f& p : origin->grid)
        grid3D.push_back(cv::Point3f(p.x, p.y, 0));

    return true;
}

bool AREngine::setNextFrame(const cv::Mat& frame)
{
    if (frame.empty()) return false;

    frameQueue.push_front(std::make_shared<ARFrame>(frame.clone()));
    if (frameQueue.size() > maxFrames)
        frameQueue.resize(maxFrames);

    detector->detectAndCompute(frameQueue.front()->frame, cv::Mat(), frameQueue.front()->keyPoints, frameQueue.front()->descriptors);

    std::vector<std::vector<cv::Point2f>> preliminaryGrids;
    // Multithreading
    std::vector<std::thread> threads(sampleQueue.size());
    std::vector<std::promise<std::vector<cv::Point2f>>> promises(sampleQueue.size());
    std::vector<std::future<std::vector<cv::Point2f>>> futures(sampleQueue.size());

    for (size_t i = 0; i < sampleQueue.size(); ++i)
    {
        threads[i] = std::thread([this, i](std::promise<std::vector<cv::Point2f>>& p)
        {
            p.set_value(std::move(frameProcessor->processing(frameQueue.front(), sampleQueue[i])));
        }, std::ref(promises[i]));
        futures[i] = promises[i].get_future();
    }

    preliminaryGrids.push_back(frameProcessor->processing(frameQueue.front(), origin));
    if (preliminaryGrids.back().empty()) preliminaryGrids.pop_back();

    for (size_t i = 0; i < threads.size(); ++i)
        threads[i].join();

    for (size_t i = 0; i < futures.size(); ++i)
    {
        preliminaryGrids.push_back(std::move(futures[i].get()));
        if (preliminaryGrids.back().empty()) preliminaryGrids.pop_back();
    }

    if (preliminaryGrids.empty()) return false;

    frameQueue.front()->found = true;
    averageGrid(preliminaryGrids, frameQueue.front());

    if (PnP(frameQueue.front()))
        isSample(frameQueue.front());

    viewer->paint(frameQueue.front());

    return true;
}

cv::Mat AREngine::getLastFrame() const
{
    if (frameQueue.empty()) return cv::Mat();
    return frameQueue.front()->frame.clone();
}
