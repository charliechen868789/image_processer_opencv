#pragma once

#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>
#include "qrcode_finder_export.hpp"

namespace qrcode {

class QRCODE_FINDER_EXPORT QRCodeFinder {
public:
    QRCodeFinder();

    // Attempts to detect and decode QR code in the image
    std::string detectAndDecode(const cv::Mat& input);

    // Draws bounding box and text on image (only if last detection succeeded)
    cv::Mat drawResult() const;

private:
    std::string decodedText;
    std::vector<cv::Point> points;
    cv::Mat lastFrame;
    bool found;
};

} // namespace qrcode
