#include "threadsafe_queue.hpp"

void ThreadsafeQueue::push(const Task& task, bool lock)
{
  if (lock)
  {
    mQueueMutex.lock();
  }

  mQueue.push(task);

  if (lock)
  {
    mQueueMutex.unlock();
  }
}

ThreadsafeQueue::Task ThreadsafeQueue::pop(bool lock)
{
  if (lock)
  {
    mQueueMutex.lock();
  }

  Task task(mQueue.front());
  mQueue.pop();
  if (lock)
  {
    mQueueMutex.unlock();
  }
  return task;
}
