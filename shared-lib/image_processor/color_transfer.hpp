#ifndef IMAGE_PROCESSOR_COLOR_TRANSFER_HPP
#define IMAGE_PROCESSOR_COLOR_TRANSFER_HPP

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace image_process {

class ColorTransfer {
public:
    ColorTransfer(const cv::Mat& styleImage, const cv::Mat& targetImage);
    cv::Mat applyTransfer();

private:
    cv::Mat styleBGR, targetBGR;
    cv::Mat styleLab, targetLab;
};

} // namespace image_process

#endif // IMAGE_PROCESSOR_COLOR_TRANSFER_HPP
