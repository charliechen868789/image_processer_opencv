// shared-lib/edge_detection.cpp
#include "edge_detection.hpp"
#include <opencv2/imgproc.hpp>

namespace image_process {

EdgeDetection::EdgeDetection(Method method) : method_(method) {}

void EdgeDetection::process(const cv::Mat& input, cv::Mat& output) {
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    switch (method_) {
        case CANNY:
            cv::Canny(gray, output, 100, 200);
            break;
        case SOBEL: {
            cv::Mat grad_x, grad_y;
            cv::Sobel(gray, grad_x, CV_16S, 1, 0);
            cv::Sobel(gray, grad_y, CV_16S, 0, 1);
            cv::Mat abs_grad_x, abs_grad_y;
            cv::convertScaleAbs(grad_x, abs_grad_x);
            cv::convertScaleAbs(grad_y, abs_grad_y);
            cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
            break;
        }
        case LAPLACIAN:
            cv::Laplacian(gray, output, CV_16S);
            cv::convertScaleAbs(output, output);
            break;
    }
}

std::string EdgeDetection::name() const {
    switch (method_) {
        case CANNY: return "Canny";
        case SOBEL: return "Sobel";
        case LAPLACIAN: return "Laplacian";
        default: return "Unknown";
    }
}

} // namespace image_process
