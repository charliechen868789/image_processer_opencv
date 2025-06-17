#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>
#include <future>
#include "image_processor/edge_detection.hpp"
#include "thread_pool/thread_pool.hpp"
#include <unordered_set>

using namespace std;
using namespace cv;
using namespace image_process;

// GUI task queue
std::queue<std::function<void()>> guiTasks;
std::mutex guiMutex;
std::condition_variable guiCond;
std::atomic<bool> guiRunning{true};
std::unordered_set<std::string> openWindows;
std::mutex openWindowsMutex;

void enqueueGuiTask(std::function<void()> task);

void enqueueGuiTask(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(guiMutex);
    guiTasks.push(std::move(task));
    guiCond.notify_one();
}

// GUI thread function
void guiThreadFunc() {
    while (guiRunning) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(guiMutex);
            if (guiTasks.empty()) {
                guiCond.wait_for(lock, std::chrono::milliseconds(30), []{ return !guiTasks.empty() || !guiRunning; });
            }
            if (!guiRunning && guiTasks.empty()) break;

            if (!guiTasks.empty()) {
                task = std::move(guiTasks.front());
                guiTasks.pop();
            }
        }

        if (task) {
            cout << "Running GUI task...\n";
            task();
        }

        int key = waitKey(300);
        if (key >= 0) {
            std::lock_guard<std::mutex> lock(openWindowsMutex);
            if (!openWindows.empty()) {
                // Close one window on any key press for simplicity
                auto it = openWindows.begin();
                destroyWindow(*it);
                cout << "Window " << *it << " closed by user key press\n";
                openWindows.erase(it);
            }
        }
    }
}

// Run edge detection in background thread
void runEdgeDetection(EdgeDetection::Method method, const Mat& input, int instanceId) {
    Mat output;
    EdgeDetection edge(method);
    edge.process(input, output);
    string windowName = edge.name() + " Result " + std::to_string(instanceId);

    // This call must be inside this function, not outside
    enqueueGuiTask([output, windowName]() {
        imshow(windowName, output);
        {
            std::lock_guard<std::mutex> lock(openWindowsMutex);
            openWindows.insert(windowName);
        }
        cout << windowName << " displayed. Press any key on that window to close it.\n";
    });
}




int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <image_path>\n";
        return 1;
    }

    Mat image = imread(argv[1]);
    if (image.empty()) {
        cerr << "Error: Cannot load image " << argv[1] << endl;
        return 1;
    }

    ThreadPool pool(std::thread::hardware_concurrency());
    std::thread guiThread(guiThreadFunc);

    int taskCount = 1;

    while (true) {
        cout << "\n=== Main Menu ===\n";
        cout << "1. Edge Detection Methods\n";
        cout << "2. Other Processing (not implemented)\n";
        cout << "3. Exit\n";
        cout << "Choose option: ";

        int mainChoice;
        cin >> mainChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (mainChoice == 3) break;

        switch (mainChoice) {
            case 1: {
                // Edge Detection submenu
                while (true) {
                    cout << "\n=== Edge Detection Menu ===\n";
                    cout << "1. Canny\n";
                    cout << "2. Sobel\n";
                    cout << "3. Laplacian\n";
                    cout << "4. Back to Main Menu\n";
                    cout << "Choose method: ";

                    int choice;
                    cin >> choice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input.\n";
                        continue;
                    }

                    if (choice == 4) break;

                    EdgeDetection::Method method;

                    switch (choice) {
                        case 1: method = EdgeDetection::CANNY; break;
                        case 2: method = EdgeDetection::SOBEL; break;
                        case 3: method = EdgeDetection::LAPLACIAN; break;
                        default:
                            cout << "Invalid choice.\n";
                            continue;
                    }

                    pool.enqueue(runEdgeDetection, method, image.clone(), taskCount++);
                    cout << "Edge detection task enqueued.\n";
                }
                break;
            }
            case 2:
                cout << "Other processing is not implemented yet.\n";
                break;
            default:
                cout << "Invalid option.\n";
                break;
        }
    }

    // Shutdown
    pool.shutdown();
    {
        std::lock_guard<std::mutex> lock(guiMutex);
        guiRunning = false;
    }
    guiCond.notify_all();
    guiThread.join();

    return 0;
}
