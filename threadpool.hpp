#pragma once

#include <vector>
#include <functional>

#include "threadsafe_queue.hpp"

class ThreadPool
{
  public:
    typedef ThreadsafeQueue::Task Task;
    
    ThreadPool(int numOfThreads);
    ~ThreadPool();
    
    void executeTask(Task task);
    bool hasFreeThreads() const;

    struct ThreadController;

  private:
    std::vector<ThreadController> mThreadControls;
    int mThreadsRunning;
    ThreadsafeQueue mTaskQueue;
};
