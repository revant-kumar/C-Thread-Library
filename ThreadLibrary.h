#ifndef THREADLIBRARY_H
#define THREADLIBRARY_H

#include <pthread.h>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>

class ThreadLibrary {
public:
    // Method to create and run threads
    void createAndRunThreads(int numThreads, std::function<void(int)> task);

    // Method to join all threads
    void joinAllThreads();

private:
    struct ThreadData {
        int threadId;
        std::function<void(int)> task;
        ThreadLibrary* library;
    };

    static void* threadFunction(void* arg);
    void signalThreadCompletion();

    std::vector<pthread_t> threads;
    std::vector<std::unique_ptr<ThreadData>> threadData;

    std::mutex mtx;
    std::condition_variable cv;
    int completedThreads = 0;
    int totalThreads = 0;
    bool allThreadsCompleted = false; // Flag to avoid spurious wake-ups
};

#endif // THREADLIBRARY_H
