#include "ARFrameProcessor.h"

#include "AREngine.h"

SetPointsPair ARFrameProcessor::match(std::shared_ptr<ARFrame> frame, std::shared_ptr<ARFrame> sample)
{
    SetPointsPair goodPoints;
    if (frame->keyPoints.empty() || frame->descriptors.empty() || sample->keyPoints.empty() || sample->descriptors.empty()) return goodPoints;

    std::vector<std::vector<cv::DMatch>> matches12, matches21;
    engine->matcher.knnMatch(frame->descriptors, sample->descriptors, matches12, 2);
    engine->matcher.knnMatch(sample->descriptors, frame->descriptors, matches21, 2);

    for (std::vector<cv::DMatch>& vec : matches12)
        if (vec[0].distance >= (vec[1].distance * 0.8))
            vec.clear();
    for (std::vector<cv::DMatch>& vec : matches21)
        if (vec[0].distance >= (vec[1].distance * 0.8))
            vec.clear();

    std::vector<cv::DMatch> goodMatches;

    for (std::vector<cv::DMatch>& vec1 : matches12)
    {
        if (vec1.empty()) continue;
        for (std::vector<cv::DMatch>& vec2 : matches21)
        {
            if (vec2.empty()) continue;
            if (vec1[0].queryIdx == vec2[0].trainIdx && vec1[0].trainIdx == vec2[0].queryIdx)
            {
                goodMatches.push_back(std::move(vec1[0]));
                break;
            }
        }
    }

    std::cout<<"Good matches: "<<goodMatches.size()<<std::endl; //
    if (goodMatches.size() > 20) // 20 matches enough?
    {
        for (cv::DMatch& match : goodMatches)
        {
            goodPoints.first.push_back(frame->keyPoints[match.queryIdx].pt);
            goodPoints.second.push_back(sample->keyPoints[match.trainIdx].pt);
        }
    }
    else
    {
        goodPoints.first.clear();
        goodPoints.second.clear();
    }

    return goodPoints;
}

std::vector<cv::Point2f> ARFrameProcessor::createGrid(const std::vector<cv::Point2f> &frameGoodPoints, const std::vector<cv::Point2f> &sampleGrid, const std::vector<cv::Point2f> &sampleGoodPoints)
{
       cv::Mat h = cv::findHomography(sampleGoodPoints, frameGoodPoints, cv::RANSAC);
       std::vector<cv::Point2f> grid;
       if (!h.empty())
       {
           cv::perspectiveTransform(sampleGrid, grid, h);
       }
       return grid;
}

bool ARFrameProcessor::checkGrid(std::vector<cv::Point2f> grid) const
{
    std::vector<cv::Point2f> contour(grid.begin(), grid.begin() + 4);

    if (!cv::isContourConvex(contour)) return false;

    std::vector<float> vecX{grid[0].x - grid[1].x, grid[1].x - grid[2].x, grid[2].x - grid[3].x, grid[3].x - grid[0].x},
                       vecY{grid[0].y - grid[1].y, grid[1].y - grid[2].y, grid[2].y - grid[3].y, grid[3].y - grid[0].y},
                       magnitudes;
    cv::magnitude(vecX, vecY, magnitudes);
    if (magnitudes[0] < magnitudes[2] * 0.8 || magnitudes[0] > magnitudes[2] * 1.2 || magnitudes[1] < magnitudes[3] * 0.8 || magnitudes[1] > magnitudes[3] * 1.2) return false;

    int minSide = engine->frameQueue.front()->frame.cols < engine->frameQueue.front()->frame.rows ?
                  engine->frameQueue.front()->frame.cols :
                  engine->frameQueue.front()->frame.rows;
    minSide *= 0.1;

    for (float& mag : magnitudes)
        if (mag <= minSide)
            return false;

    std::shared_ptr<ARFrame> sample;
    if (engine->frameQueue.size() > 1 && engine->frameQueue[1]->found)
    {
        sample = engine->frameQueue[1];
    }
    else if (engine->frameQueue.size() > 2 && engine->frameQueue[2]->found)
    {
        sample = engine->frameQueue[2];
    }
    if (sample)
    {
        for (int i = 0; i < grid.size(); i += 3)
            if (std::abs(grid[i].x - sample->grid[i].x) > engine->frameQueue.front()->frame.cols * 0.05 ||
                std::abs(grid[i].y - sample->grid[i].y) > engine->frameQueue.front()->frame.rows * 0.05) return false;
    }

    return true;
}

ARFrameProcessor::ARFrameProcessor(AREngine *engine)
{
    this->engine = engine;
}

ARFrameProcessor *ARFrameProcessor::clone() const
{
    return new ARFrameProcessor(this->engine);
}

std::vector<cv::Point2f> ARFrameProcessor::processing(std::shared_ptr<ARFrame> frame, std::shared_ptr<ARFrame> sample)
{
    SetPointsPair matchGoodPoints = match(frame, sample);

    std::vector<cv::Point2f> grid;
    if (!matchGoodPoints.first.empty())
    {
        grid = createGrid(matchGoodPoints.first, sample->grid, matchGoodPoints.second);

        if (!grid.empty())
        {
            if (!checkGrid(grid))
            {
                grid.clear();
            }
        }
    }
    return grid;
}
