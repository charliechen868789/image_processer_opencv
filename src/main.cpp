#include "edge_detection.hpp"
#include "thread_pool.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <future>

using namespace image_process;

int main(int argc, char** argv) {
    if (argc < 2) return 1;

    cv::Mat img = cv::imread(argv[1]);
    if (img.empty()) return 1;

    ThreadPool pool(std::thread::hardware_concurrency());

    // Enqueue the edge detection task
    auto future = pool.enqueue([&img]() -> cv::Mat {
        EdgeDetection edge(EdgeDetection::SOBEL);
        cv::Mat output;
        edge.process(img, output);
        return output;  // Return processed image
    });

    // Wait for the task to finish and get the result
    cv::Mat result = future.get();

    // Show the result in main thread (GUI stuff should be in main thread)
    EdgeDetection edge(EdgeDetection::SOBEL);
    cv::imshow(edge.name(), result);
    cv::waitKey(0);

    pool.shutdown();

    return 0;
}
