#include <iostream>
#include <chrono>
#include "ThreadLibrary.h"

void basicFunction(int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void spawnThreadsWithPthread(int n, double& duration) {
    ThreadLibrary threadLib;

    auto task = [](int id) { basicFunction(id); };

    auto start = std::chrono::high_resolution_clock::now();
    threadLib.createAndRunThreads(n, task);
    threadLib.joinAllThreads();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - start;
    duration = dur.count();

    std::cout << "Time taken with pthread: " << duration << " seconds" << std::endl;
}

void spawnThreadsWithStdThread(int n, double& duration) {
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        threads.emplace_back(basicFunction, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - start;
    duration = dur.count();

    std::cout << "Time taken with std::thread: " << duration << " seconds" << std::endl;
}

int main() {
    const int numThreads = 1000;
    double durationPthread = 0.0;
    double durationStdThread = 0.0;

    spawnThreadsWithPthread(numThreads, durationPthread);
    spawnThreadsWithStdThread(numThreads, durationStdThread);

    double percentDifference = ((durationPthread - durationStdThread) / durationStdThread) * 100.0;
    std::cout << "pthread is " << percentDifference << "% " << (percentDifference > 0 ? "slower" : "faster") << " than std::thread" << std::endl;

    return 0;
}
