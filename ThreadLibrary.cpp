#include "ThreadLibrary.h"
#include <iostream>

void ThreadLibrary::createAndRunThreads(int numThreads, std::function<void(int)> task) {
    for (int i = 0; i < numThreads; ++i) {
        auto data = std::make_unique<ThreadData>();
        data->threadId = i;
        data->task = task;

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
    return nullptr;
}

void ThreadLibrary::joinAllThreads() {
    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }
    threads.clear();
    threadData.clear();
}
