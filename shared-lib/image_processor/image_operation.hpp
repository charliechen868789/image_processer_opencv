// shared-lib/image_operation.hpp
#ifndef IMAGE_OPERATION_HPP
#define IMAGE_OPERATION_HPP

#include <opencv2/core.hpp>
#include <string>

namespace image_process {

class ImageOperation {
public:
    virtual ~ImageOperation() = default;

    virtual void process(const cv::Mat& input, cv::Mat& output) = 0;

    virtual std::string name() const = 0;
};

} // namespace image_process

#endif // IMAGE_OPERATION_HPP
