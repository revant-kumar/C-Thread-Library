#include <iostream>
#include "ThreadLibrary.h"

void printMessage(int threadId) {
    std::cout << "Hello from thread " << threadId << std::endl;
}

int main() {
    ThreadLibrary threadLib;

    // Create and run 5 threads
    threadLib.createAndRunThreads(5, printMessage);

    // Join all threads
    threadLib.joinAllThreads();

    std::cout << "All threads have finished execution." << std::endl;

    return 0;
}
