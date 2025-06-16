#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t threadCount) : stop(false) {
    for (size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this]() { 
                        return this->stop.load() || !this->tasks.empty(); 
                    });

                    if (this->stop.load() && this->tasks.empty())
                        return;

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::shutdown() {
    stop.store(true);
    condition.notify_all();

    for (std::thread &worker : workers) {
        if (worker.joinable())
            worker.join();
    }
}
