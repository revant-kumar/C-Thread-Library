## Custom Thread Library

### Introduction

This project demonstrates the implementation of a custom thread library using POSIX threads (`pthread`). The custom library is designed to provide low-level control over thread management and synchronization, offering functionality similar to the C++ Standard Library's `<thread>` library. The custom thread library includes various features like mutexes, lock guards, condition variables, thread attributes, and thread-local storage, making it robust and flexible for multithreaded programming.

### Platforms

The custom thread library is designed to work on Unix-like systems that support POSIX threads, such as:

- **Linux**
- **macOS**
- **BSD variants**

It can also be adapted for use on other platforms that support POSIX threads. For Windows, modifications would be necessary to use the native Windows threading API.

### Features

1. **Thread Creation and Management**:
   - Create and run multiple threads with specified tasks.
   - Join all threads to ensure proper cleanup and synchronization.

2. **Mutexes and Lock Guards**:
   - Protect shared resources from concurrent access by multiple threads.
   - Automatic locking and unlocking of mutexes to prevent deadlocks and ensure safe access.

3. **Condition Variables**:
   - Coordinate thread activities by allowing threads to wait for specific conditions to be met.
   - Notify one or all waiting threads when conditions change.

4. **Thread Attributes**:
   - Customize thread behavior by setting attributes such as stack size and detach state.

5. **Thread-Local Storage (TLS)**:
   - Provide each thread with its own instance of a variable, avoiding conflicts and ensuring thread-specific data is maintained separately.

### Benchmarks

To evaluate the performance of the custom thread library, we compare it with the standard C++ `<thread>` library. The benchmark measures the time taken to create, run, and join a specified number of threads.

**Benchmark Code**: `benchmark.cpp`

```cpp
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
    int numThreads = 1000;  // Number of threads to create

    std::cout << "Benchmarking with " << numThreads << " threads:" << std::endl;

    // Benchmark custom thread library
    benchmarkCustomThreadLibrary(numThreads);

    // Benchmark std::thread library
    benchmarkStdThreadLibrary(numThreads);

    return 0;
}
```
#### Result:

![image](https://github.com/revant-kumar/C-Thread-Library/assets/114937643/2e8df9cb-0336-40a9-82eb-d1766067611d)

**Note:** The results may vary as they are highly dependent on the machine, the environment, and other simultaneous processes.

### How Is It Different from the Inbuilt `<thread>` Library?

While both the custom thread library and the C++ `<thread>` library provide multithreading capabilities, there are several key differences:

1. **Low-Level Control**:
   - The custom thread library provides lower-level control over thread creation and management using POSIX threads, allowing for more fine-grained customization.

2. **Custom Features**:
   - The custom library includes additional features like custom mutexes, condition variables, thread attributes, and thread-local storage, which are designed to mimic and enhance the capabilities of the standard library.

3. **Learning and Flexibility**:
   - Implementing and using the custom thread library offers a deeper understanding of threading concepts and POSIX threads, providing a learning experience and flexibility for specific use cases.

4. **Performance**:
   - The performance can vary based on the specific implementation and use case. Benchmarking helps to evaluate the differences in execution time between the custom library and the `<thread>` library.

### Detailed Explanation of Each Feature

#### Thread Creation and Management

**Functionality**:
- Create and run multiple threads by specifying the number of threads and the task each thread should execute.
- Ensure proper synchronization and cleanup by joining all threads.

**Example Usage**:
```cpp
ThreadLibrary threadLib;
threadLib.createAndRunThreads(5, printMessage);
threadLib.joinAllThreads();
```

#### Mutexes and Lock Guards

**Functionality**:
- Use mutexes to protect shared resources and ensure only one thread accesses a resource at a time.
- Use lock guards to automatically manage mutex locking and unlocking.

**Example Usage**:
```cpp
ThreadLibrary::Mutex mtx;
ThreadLibrary::LockGuard lock(mtx);
// Critical section
```

#### Condition Variables

**Functionality**:
- Use condition variables to synchronize threads by making threads wait for certain conditions to be met.
- Notify one or all waiting threads when conditions change.

**Example Usage**:
```cpp
ThreadLibrary::ConditionVariable cv;
ThreadLibrary::Mutex mtx;
bool ready = false;

void waitFunction() {
    ThreadLibrary::LockGuard lock(mtx);
    while (!ready) {
        cv.wait(mtx);
    }
    // Proceed after notification
}

void notifyFunction() {
    {
        ThreadLibrary::LockGuard lock(mtx);
        ready = true;
    }
    cv.notify_all();
}
```

#### Thread Attributes

**Functionality**:
- Customize thread behavior by setting attributes such as stack size and detach state.

**Example Usage**:
```cpp
ThreadLibrary::ThreadAttributes attrs;
attrs.setStackSize(1024 * 1024); // 1 MB stack size
attrs.setDetachState(true); // Detached state
```

#### Thread-Local Storage (TLS)

**Functionality**:
- Provide each thread with its own instance of a variable, ensuring thread-specific data is maintained separately.

**Example Usage**:
```cpp
ThreadLibrary::ThreadLocal<int> threadLocalData;

void threadFunction(int threadId) {
    threadLocalData.set(threadId);
    std::cout << "Thread " << threadId << " local data: " << threadLocalData.get() << std::endl;
}
```

### How to Use the Custom Thread Library

1. **Include the Header File**:
   Make sure to include `ThreadLibrary.h` in your project.

2. **Compile the Code**:
   Compile the project using a C++ compiler that supports POSIX threads and C++14 or later.
   ```sh
   g++ -o main main.cpp ThreadLibrary.cpp -lpthread -std=c++14
   ```

3. **Run the Executable**:
   After compiling, run the executable to see the output.
   ```sh
   ./main
   ```

### Conclusion

The custom thread library provides a robust and flexible alternative to the C++ `<thread>` library, with additional features and low-level control. It serves as an excellent learning tool for understanding multithreading concepts and POSIX threads, and it can be customized for specific use cases. By comparing its performance with the standard library through benchmarks, you can evaluate the efficiency and suitability of the custom implementation for your projects.
