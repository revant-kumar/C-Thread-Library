#include <iostream>
#include <map>
#include "ThreadLibrary.h"

using namespace std;

void printMaps(const map<string, int>& m) {
    for (const auto& i : m) {
        cout << i.first << " " << i.second << endl;
    }
}

int main() {
    ThreadLibrary threadLib;
    map<string, int> mp;

    mp["salary"] = 3500000;
    mp["age"] = 22;
    mp["ID"] = 310;

    // Use a lambda function to bind the map to the printMaps function
    auto task = [&mp](int) { printMaps(mp); };

    threadLib.createAndRunThreads(2, task);
    cout<<"Intermediate process 1"<<endl;
    cout<<"Intermediate process 2"<<endl;
    threadLib.joinAllThreads();

    return 0;
}
