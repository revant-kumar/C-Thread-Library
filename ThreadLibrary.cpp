#include "ThreadLibrary.h"
#include <iostream>

void ThreadLibrary::createAndRunThreads(int numThreads, std::function<void(int)> task) {
    totalThreads = numThreads;
    completedThreads = 0;
    allThreadsCompleted = false; // Reset flag

    for (int i = 0; i < numThreads; ++i) {
        auto data = std::make_unique<ThreadData>();
        data->threadId = i;
        data->task = task;
        data->library = this;

        pthread_t thread;
        int rc = pthread_create(&thread, nullptr, ThreadLibrary::threadFunction, data.get());
        if (rc) {
            std::cerr << "Error: unable to create thread, " << rc << std::endl;
            exit(-1);
        }

        threads.push_back(thread);
        threadData.push_back(std::move(data));
    }
}

void* ThreadLibrary::threadFunction(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    data->task(data->threadId);
    data->library->signalThreadCompletion();
    return nullptr;
}

void ThreadLibrary::signalThreadCompletion() {
    std::unique_lock<std::mutex> lock(mtx);
    completedThreads++;
    if (completedThreads == totalThreads) {
        allThreadsCompleted = true;
        cv.notify_all();
    }
}

void ThreadLibrary::joinAllThreads() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return allThreadsCompleted; });

    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }
    threads.clear();
    threadData.clear();
}
