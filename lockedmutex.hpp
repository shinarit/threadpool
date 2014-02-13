#pragma once

#include <mutex>

class LockedMutex
{
  public:
    LockedMutex()
    {
      mMutex.lock();
    }
    
    void lock()
    {
      mMutex.lock();
    }
    void unlock()
    {
      mMutex.unlock();
    }
    
  private:
    std::mutex mMutex;
};
