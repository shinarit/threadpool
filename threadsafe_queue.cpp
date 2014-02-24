#include "threadsafe_queue.hpp"

void ThreadsafeQueue::push(const Task& task)
{
  Lock lock(mQueueMutex);

  mQueue.push(task);
}

ThreadsafeQueue::Task ThreadsafeQueue::pop()
{
  Lock lock(mQueueMutex);

  Task task(mQueue.front());
  mQueue.pop();
  return task;
}
