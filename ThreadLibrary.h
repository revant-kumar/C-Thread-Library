#ifndef THREADLIBRARY_H
#define THREADLIBRARY_H

#include <pthread.h>
#include <vector>
#include <functional>
#include <memory>

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
    };

    static void* threadFunction(void* arg);
    std::vector<pthread_t> threads;
    std::vector<std::unique_ptr<ThreadData>> threadData;
};

#endif // THREADLIBRARY_H
