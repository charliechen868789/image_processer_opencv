#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ContrastEnhancer {
public:
    ContrastEnhancer(const cv::Mat& inputImage);
    void enhanceAndShow(const std::string& windowPrefix);

private:
    cv::Mat originalImage;
    cv::Mat enhancedImage;
    std::vector<cv::Mat> originalHist;
    std::vector<cv::Mat> enhancedHist;

    void computeHistogram(const cv::Mat& channel, cv::Mat& hist);
    void drawHistogram(const cv::Mat& hist, const std::string& windowName);
    void stretchContrast(cv::Mat& channel);
};
