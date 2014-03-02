#pragma once

#include <mutex>
#include <queue>

class ThreadsafeQueue
{
  public:
    typedef std::function<void()> Task;

    void push(const Task&, bool lock = true);
    Task pop(bool lock = true);
    bool empty() const
    {
      return mQueue.empty();
    }
    void lock()
    {
      mQueueMutex.lock();
    }
    void unlock()
    {
      mQueueMutex.unlock();
    }

  private:
    typedef std::lock_guard<std::mutex> Lock;
    std::queue<Task> mQueue;
    std::mutex mQueueMutex;
};
