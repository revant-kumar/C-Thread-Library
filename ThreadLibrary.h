#ifndef THREADLIBRARY_H
#define THREADLIBRARY_H

#include <pthread.h>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>

class ThreadLibrary {
public:
    // Method to create and run threads
    void createAndRunThreads(int numThreads, std::function<void(int)> task);

    // Method to join all threads
    void joinAllThreads();

    // Mutex class
    class Mutex {
    public:
        Mutex();
        ~Mutex();
        void lock();
        void unlock();
        pthread_mutex_t* getPthreadMutex();
    private:
        pthread_mutex_t mutex;
    };


    //Thread Attributes
    class ThreadAttributes {
    public:
        ThreadAttributes();
        ~ThreadAttributes();
        void setStackSize(size_t size);
        void setDetachState(bool detached);

        pthread_attr_t* getPthreadAttr();

    private:
        pthread_attr_t attr;
    };


    // Lock guard class
    class LockGuard {
    public:
        LockGuard(Mutex& m);
        ~LockGuard();

    private:
        Mutex& mutex;
    };

    //Condition Variable
    class ConditionVariable {
    public:
        ConditionVariable();
        ~ConditionVariable();
        void wait(Mutex& m);
        void notify_one();
        void notify_all();

    private:
        pthread_cond_t cond;
    };

    
    //Thread Local Storage (TLS)
    template <typename T>
    class ThreadLocal {
    public:
        ThreadLocal();
        ~ThreadLocal();
        T& get();
        void set(const T& value);

    private:
        pthread_key_t key;
        static void destructor(void* value);
    };


private:
    struct ThreadData {
        int threadId;
        std::function<void(int)> task;
        ThreadLibrary* library;
    };

    static void* threadFunction(void* arg);
    void signalThreadCompletion();

    std::vector<pthread_t> threads;
    std::vector<std::unique_ptr<ThreadData>> threadData;

    std::mutex mtx;
    std::condition_variable cv;
    int completedThreads = 0;
    int totalThreads = 0;
    bool allThreadsCompleted = false; // Flag to avoid spurious wake-ups
};



#endif // THREADLIBRARY_H
