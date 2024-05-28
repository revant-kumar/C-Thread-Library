#include <iostream>
#include <vector>
#include "ThreadLibrary.h"

using namespace std;

void doSomething(int id) {
    cout << id << endl;
}

void spawnThreads(int n)
{
    ThreadLibrary threadLib;

    // Use a lambda function to wrap the doSomething function
    auto task = [](int id) { doSomething(id + 1); };

    // Create and run threads
    threadLib.createAndRunThreads(n, task);

    // Join all threads
    threadLib.joinAllThreads();
}

int main()
{
    spawnThreads(10);
    return 0;
}
