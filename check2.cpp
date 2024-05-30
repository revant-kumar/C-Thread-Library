#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>
#include <numeric>  // For std::accumulate

void printMessage(int threadId) {
    std::cout << "Hello from thread " << threadId << std::endl;
}

void createAndRunThreads(int numThreads, std::function<void(int)> task) {
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(task, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}

double measureExecutionTime(int numThreads, std::function<void(int)> task) {
    auto start = std::chrono::high_resolution_clock::now();

    // Create and run threads
    createAndRunThreads(numThreads, task);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    return elapsed_seconds.count();
}

int main() {
    const int numExecutions = 99999; // Number of times to execute
    const int numThreads = 5; // Number of threads to create each execution
    std::vector<double> executionTimes;

    for (int i = 0; i < numExecutions; ++i) {
        double elapsed = measureExecutionTime(numThreads, printMessage);
        executionTimes.push_back(elapsed);
        std::cout << "Execution " << i + 1 << " time: " << elapsed << "s" << std::endl;
    }

    double total = std::accumulate(executionTimes.begin(), executionTimes.end(), 0.0);
    double average = total / numExecutions;

    std::cout << "All executions have finished." << std::endl;
    std::cout << "Average elapsed time: " << average << "s\n";

    return 0;
}
