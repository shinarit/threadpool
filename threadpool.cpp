#include "threadpool.hpp"

#include <thread>
#include <algorithm>
#include "lockedmutex.hpp"

#include <iostream>

class ThreadPool::ThreadController;

void executer(ThreadPool::ThreadController* threadController);

void nop()
{
  std::cerr << "xd\n";
}

struct ThreadPool::ThreadController
{
  ThreadController(): toRun(nop), myThread(std::bind(executer, this))
  { }
  void setNextTask()
  {
    ThreadsafeQueue& queue(mPool->mTaskQueue);
    queue.lock();
    if (queue.empty())
    {
      runningTask = false;
    }
    else
    {
      Task task(queue.pop(false));
      toRun = task;
      runMutex.unlock();
    }
    queue.unlock();
  }

  typedef LockedMutex Mutex;
  Mutex runMutex;
  Task toRun;
  std::thread myThread;
  bool exit = false;
  bool runningTask = false;
  ThreadPool* mPool;
};

void executer(ThreadPool::ThreadController* threadController)
{
  ThreadPool::ThreadController& tc(*threadController);
  while (!tc.exit)
  {
    tc.runMutex.lock();
    if (tc.runningTask)
    {
      tc.toRun();
    }
    tc.setNextTask();
  }
}


ThreadPool::ThreadPool(int numOfThreads): mThreadControls(numOfThreads)
{
  for (auto& tc: mThreadControls)
  {
    tc.mPool = this;
  }
}

ThreadPool::~ThreadPool()
{
  for (auto& tc: mThreadControls)
  {
    tc.exit = true;
    tc.runningTask = false;
    tc.runMutex.unlock();
  }
  for (auto& tc: mThreadControls)
  {
    tc.myThread.join();
  }
}

void ThreadPool::executeTask(Task task)
{
  mTaskQueue.lock();
  auto it = std::find_if(mThreadControls.begin(), mThreadControls.end(), [](const ThreadController& tc) { return !tc.runningTask; } );
  if (it != mThreadControls.end())
  {
    ThreadController& tc(*it);
    tc.toRun = task;
    tc.runningTask = true;
    tc.runMutex.unlock();
  }
  else
  {
    mTaskQueue.push(task, false);
  }
  mTaskQueue.unlock();
}
