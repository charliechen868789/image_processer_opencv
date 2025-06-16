// shared-lib/edge_detection.hpp
#ifndef EDGE_DETECTION_HPP
#define EDGE_DETECTION_HPP

#include "image_operation.hpp"
#include <opencv2/core.hpp>

namespace image_process {

class EdgeDetection : public ImageOperation {
public:
    enum Method {
        CANNY,
        SOBEL,
        LAPLACIAN
    };

    explicit EdgeDetection(Method method = CANNY);
    void process(const cv::Mat& input, cv::Mat& output) override;
    std::string name() const override;

private:
    Method method_;
};

} // namespace image_process

#endif // EDGE_DETECTION_HPP
