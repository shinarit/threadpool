#pragma once

#include <mutex>
#include <queue>

class ThreadsafeQueue
{
  public:
    typedef std::function<void()> Task;

    void push(const Task&);
    Task pop();
    bool empty() const
    {
      return mQueue.empty();
    }

  private:
    typedef std::unique_lock<std::mutex> Lock;
    std::queue<Task> mQueue;
    std::mutex mQueueMutex;
};
