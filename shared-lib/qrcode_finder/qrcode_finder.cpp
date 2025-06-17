#include "qrcode_finder.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

namespace qrcode_finder {

QRCodeFinder::QRCodeFinder()
    : found(false) {}

std::string QRCodeFinder::detectAndDecode(const cv::Mat& input) {
    lastFrame = input.clone();
    points.clear();
    decodedText.clear();
    found = false;

    cv::QRCodeDetector qrDetector;
    std::vector<cv::Point> corners;
    std::string result = qrDetector.detectAndDecode(input, corners);

    if (!result.empty() && corners.size() == 4) {
        found = true;
        decodedText = result;
        points = corners;
    }

    return decodedText;
}

cv::Mat QRCodeFinder::drawResult() const {
    cv::Mat output = lastFrame.clone();

    if (found && points.size() == 4) {
        for (int i = 0; i < 4; ++i) {
            cv::line(output, points[i], points[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
        }
        cv::putText(output, decodedText, points[0] + cv::Point(5, -10), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 0), 2);
    }

    return output;
}

} // namespace qrcode
