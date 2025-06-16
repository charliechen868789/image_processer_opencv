#include "edge_detection.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace image_process;

int main(int argc, char** argv) {
    if (argc < 2) return 1;

    cv::Mat img = cv::imread(argv[1]);
    if (img.empty()) return 1;

    EdgeDetection edge(EdgeDetection::SOBEL);
    cv::Mat output;
    edge.process(img, output);

    cv::imshow(edge.name(), output);
    cv::waitKey(0);

    return 0;
}
