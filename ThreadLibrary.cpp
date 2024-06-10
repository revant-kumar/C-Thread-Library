#include "ThreadLibrary.h"
#include <iostream>

// Create and run threads
void ThreadLibrary::createAndRunThreads(int numThreads, std::function<void(int)> task) {
    totalThreads = numThreads;
    completedThreads = 0;
    allThreadsCompleted = false; // Reset flag

    for (int i = 0; i < numThreads; ++i) {
        auto data = std::make_unique<ThreadData>();
        data->threadId = i;
        data->task = task;
        data->library = this;

        pthread_t thread;
        int rc = pthread_create(&thread, nullptr, ThreadLibrary::threadFunction, data.get());
        if (rc) {
            std::cerr << "Error: unable to create thread, " << rc << std::endl;
            exit(-1);
        }

        threads.push_back(thread);
        threadData.push_back(std::move(data));
    }
}

// Thread function
void* ThreadLibrary::threadFunction(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    data->task(data->threadId);
    data->library->signalThreadCompletion();
    return nullptr;
}

// Signal thread completion
void ThreadLibrary::signalThreadCompletion() {
    std::unique_lock<std::mutex> lock(mtx);
    completedThreads++;
    if (completedThreads == totalThreads) {
        allThreadsCompleted = true;
        cv.notify_all();
    }
}

// Join all threads
void ThreadLibrary::joinAllThreads() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return allThreadsCompleted; });

    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }
    threads.clear();
    threadData.clear();
}

// Mutex implementation
ThreadLibrary::Mutex::Mutex() {
    pthread_mutex_init(&mutex, nullptr);
}

ThreadLibrary::Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);
}

void ThreadLibrary::Mutex::lock() {
    pthread_mutex_lock(&mutex);
}

void ThreadLibrary::Mutex::unlock() {
    pthread_mutex_unlock(&mutex);
}

pthread_mutex_t* ThreadLibrary::Mutex::getPthreadMutex() {
    return &mutex;
}

// LockGuard implementation
ThreadLibrary::LockGuard::LockGuard(Mutex& m) : mutex(m) {
    mutex.lock();
}

ThreadLibrary::LockGuard::~LockGuard() {
    mutex.unlock();
}

// ConditionVariable implementation
ThreadLibrary::ConditionVariable::ConditionVariable() {
    pthread_cond_init(&cond, nullptr);
}

ThreadLibrary::ConditionVariable::~ConditionVariable() {
    pthread_cond_destroy(&cond);
}

void ThreadLibrary::ConditionVariable::wait(Mutex& m) {
    pthread_cond_wait(&cond, m.getPthreadMutex());
}

void ThreadLibrary::ConditionVariable::notify_one() {
    pthread_cond_signal(&cond);
}

void ThreadLibrary::ConditionVariable::notify_all() {
    pthread_cond_broadcast(&cond);
}

// ThreadAttributes implementation
ThreadLibrary::ThreadAttributes::ThreadAttributes() {
    pthread_attr_init(&attr);
}

ThreadLibrary::ThreadAttributes::~ThreadAttributes() {
    pthread_attr_destroy(&attr);
}

void ThreadLibrary::ThreadAttributes::setStackSize(size_t size) {
    pthread_attr_setstacksize(&attr, size);
}

void ThreadLibrary::ThreadAttributes::setDetachState(bool detached) {
    pthread_attr_setdetachstate(&attr, detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
}

pthread_attr_t* ThreadLibrary::ThreadAttributes::getPthreadAttr() {
    return &attr;
}

// ThreadLocal implementation
template <typename T>
ThreadLibrary::ThreadLocal<T>::ThreadLocal() {
    pthread_key_create(&key, destructor);
}

template <typename T>
ThreadLibrary::ThreadLocal<T>::~ThreadLocal() {
    pthread_key_delete(key);
}

template <typename T>
T& ThreadLibrary::ThreadLocal<T>::get() {
    T* value = static_cast<T*>(pthread_getspecific(key));
    if (!value) {
        value = new T();
        pthread_setspecific(key, value);
    }
    return *value;
}

template <typename T>
void ThreadLibrary::ThreadLocal<T>::set(const T& value) {
    T* ptr = static_cast<T*>(pthread_getspecific(key));
    if (!ptr) {
        ptr = new T(value);
        pthread_setspecific(key, ptr);
    } else {
        *ptr = value;
    }
}

template <typename T>
void ThreadLibrary::ThreadLocal<T>::destructor(void* value) {
    delete static_cast<T*>(value);
}

// Explicit template instantiation for ThreadLocal<int>
template class ThreadLibrary::ThreadLocal<int>;
