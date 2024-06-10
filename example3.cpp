#include "ThreadLibrary.h"
#include <iostream>
#include <thread>
#include <vector>
#include <functional>

ThreadLibrary::Mutex mtx;
ThreadLibrary::ConditionVariable cv;
bool ready = false;

void waitFunction(int threadId) {
    ThreadLibrary::LockGuard lock(mtx);
    while (!ready) {
        cv.wait(mtx);
    }
    std::cout << "Thread " << threadId << " proceeding after notification." << std::endl;
}

void notifyFunction() {
    {
        ThreadLibrary::LockGuard lock(mtx);
        ready = true;
    }
    cv.notify_all();
}

int main() {
    ThreadLibrary threadLib;

    // Create and run threads
    threadLib.createAndRunThreads(5, waitFunction);

    // Simulate some work in main thread
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Notify all waiting threads
    notifyFunction();

    // Join all threads
    threadLib.joinAllThreads();

    return 0;
}
