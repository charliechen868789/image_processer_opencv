#include "color_transfer.hpp"

namespace image_process {

ColorTransfer::ColorTransfer(const cv::Mat& styleImage, const cv::Mat& targetImage)
    : styleBGR(styleImage.clone()), targetBGR(targetImage.clone())
{
    cv::cvtColor(styleBGR, styleLab, cv::COLOR_BGR2Lab);
    cv::cvtColor(targetBGR, targetLab, cv::COLOR_BGR2Lab);
    styleLab.convertTo(styleLab, CV_32FC3);
    targetLab.convertTo(targetLab, CV_32FC3);
}

cv::Mat ColorTransfer::applyTransfer()
{
    cv::Mat meanStyle, stdStyle, meanTarget, stdTarget;
    cv::meanStdDev(styleLab, meanStyle, stdStyle);
    cv::meanStdDev(targetLab, meanTarget, stdTarget);

    std::vector<cv::Mat> targetChs;
    cv::split(targetLab, targetChs);

    for (int i = 0; i < 3; ++i) {
        targetChs[i] -= meanTarget.at<double>(i);
        targetChs[i] /= stdTarget.at<double>(i);
        targetChs[i] *= stdStyle.at<double>(i);
        targetChs[i] += meanStyle.at<double>(i);
    }

    cv::Mat merged, result;
    cv::merge(targetChs, merged);
    merged.convertTo(merged, CV_8UC3);
    cv::cvtColor(merged, result, cv::COLOR_Lab2BGR);
    return result;
}

} // namespace image_process
