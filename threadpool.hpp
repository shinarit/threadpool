#pragma once

#include <vector>
#include <functional>

class ThreadPool
{
  public:
    typedef std::function<void()> Task;
    
    ThreadPool(int numOfThreads);
    ~ThreadPool();
    
    void executeTask(Task task);

    struct ThreadController;
  private:
    std::vector<ThreadController> mThreadControls;
};
