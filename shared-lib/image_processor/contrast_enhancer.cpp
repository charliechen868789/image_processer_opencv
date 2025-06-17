#include "contrast_enhancer.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

ContrastEnhancer::ContrastEnhancer(const cv::Mat& inputImage)
    : originalImage(inputImage.clone()) {}

void ContrastEnhancer::computeHistogram(const Mat& channel, Mat& hist) {
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

    calcHist(&channel, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    normalize(hist, hist, 0, 400, NORM_MINMAX);
}

void ContrastEnhancer::drawHistogram(const Mat& hist, const std::string& windowName) {
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / 256);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255,255,255));

    for (int i = 1; i < 256; i++) {
        line(histImage,
             Point(bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1))),
             Point(bin_w*i, hist_h - cvRound(hist.at<float>(i))),
             Scalar(0, 0, 255), 2);
    }

    imshow(windowName, histImage);
}

void ContrastEnhancer::stretchContrast(Mat& channel) {
    double minVal, maxVal;
    minMaxLoc(channel, &minVal, &maxVal);

    uchar lut[256];
    for (int i = 0; i < 256; ++i) {
        lut[i] = saturate_cast<uchar>(255.0 * ((i / (maxVal - minVal)) - (minVal / (maxVal - minVal))));
    }

    for (int i = 0; i < channel.rows; i++) {
        uchar* rowPtr = channel.ptr<uchar>(i);
        for (int j = 0; j < channel.cols; j++) {
            rowPtr[j] = lut[rowPtr[j]];
        }
    }
}

void ContrastEnhancer::enhanceAndShow(const std::string& windowPrefix) {
    if (originalImage.empty()) return;

    // Convert to YCrCb and split channels
    Mat ycrcb;
    cvtColor(originalImage, ycrcb, COLOR_BGR2YCrCb);
    std::vector<Mat> channels;
    split(ycrcb, channels);

    // Compute original histogram
    Mat origHist;
    computeHistogram(channels[0], origHist);

    // Stretch contrast
    stretchContrast(channels[0]);

    // Compute enhanced histogram
    Mat enhHist;
    computeHistogram(channels[0], enhHist);

    // Merge channels and convert back to BGR
    merge(channels, ycrcb);
    cvtColor(ycrcb, enhancedImage, COLOR_YCrCb2BGR);

    // Display histograms and result
    drawHistogram(origHist, windowPrefix + " - Original Histogram");
    drawHistogram(enhHist, windowPrefix + " - Enhanced Histogram");
    imshow(windowPrefix + " - Enhanced Image", enhancedImage);
}
