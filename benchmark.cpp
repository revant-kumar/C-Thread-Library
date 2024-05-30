#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>
#include "ThreadLibrary.h"

// Function to be executed by each thread
void printMessage(int threadId) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Simulate work
}

// Function to benchmark the custom thread library
void benchmarkCustomThreadLibrary(int numThreads) {
    ThreadLibrary threadLib;

    auto start = std::chrono::high_resolution_clock::now();

    // Create and run threads
    threadLib.createAndRunThreads(numThreads, printMessage);

    // Join all threads
    threadLib.joinAllThreads();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time taken with custom thread library: " << elapsed_seconds.count() << " seconds" << std::endl;
}

// Function to benchmark the C++ <thread> library
void benchmarkStdThreadLibrary(int numThreads) {
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    // Create and run threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(printMessage, i);
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time taken with std::thread library: " << elapsed_seconds.count() << " seconds" << std::endl;
}

int main() {
    int numThreads = 9999;  // Number of threads to create

    std::cout << "Benchmarking with " << numThreads << " threads:" << std::endl;

    // Benchmark std::thread library
    benchmarkStdThreadLibrary(numThreads);
    
    // Benchmark custom thread library
    benchmarkCustomThreadLibrary(numThreads);


    return 0;
}
