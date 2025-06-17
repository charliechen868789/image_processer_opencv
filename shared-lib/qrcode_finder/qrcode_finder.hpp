#include <opencv2/core.hpp>
#include <string>
#include <vector>


namespace qrcode_finder 
{

class QRCodeFinder 
{
public:
    QRCodeFinder();

    // Processes a new input image and tries to decode any QR code
    std::string detectAndDecode(const cv::Mat& input);

    // Returns annotated image (call after detectAndDecode)
    cv::Mat drawResult() const;

private:
    std::string decodedText;
    std::vector<cv::Point> points;
    cv::Mat lastFrame;
    bool found;
};

} // namespace qrcode
