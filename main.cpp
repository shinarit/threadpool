#include <iostream>
#include <functional>

#include <thread>
#include <chrono>
#include <mutex>

#include "threadpool.hpp"

std::mutex xMut;

int x = 0;

void alma()
{
  std::unique_lock<std::mutex> lock(xMut);
  ++x;
  std::cout << "alma lolxd\n";
}

void korte(int n)
{
  std::unique_lock<std::mutex> lock(xMut);
  ++x;
  std::cout << "korte " << n << " lolxd\n";
}

struct X
{
  void alma()
  {
    std::unique_lock<std::mutex> lock(xMut);
    ++::x;
    std::cout << "member alma " << x << " lolxd\n";
  }
  void korte(int n)
  {
    std::unique_lock<std::mutex> lock(xMut);
    ++::x;
    std::cout << "korte member " << n << ' ' << x << " lolxd\n";
  }
  int x;
};

void waitforit(int amount)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(amount));
}

int main()
{
  X x{10};
  typedef ThreadPool::Task Task;
  Task f1 = std::bind(alma);
  Task f2 = std::bind(korte, 2);
  Task f3 = std::bind(&X::alma, &x);
  Task f4 = std::bind(&X::korte, &x, 2);

  ThreadPool tp(2);

  std::cerr << "free threads 1: " << tp.hasFreeThreads() << '\n';

  for (int i(0); i<10; ++i)
  {
    tp.executeTask(f1);
    tp.executeTask(f2);
    tp.executeTask(f3);
    tp.executeTask(f4);
  }
  
  std::cerr << "free threads 0: " << tp.hasFreeThreads() << '\n';

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::cerr << "free threads 1: " << tp.hasFreeThreads() << '\n';

  ThreadPool tp2(5);
  Task fwait = std::bind(waitforit, 1000);
  tp2.executeTask(fwait);
  tp2.executeTask(fwait);
  tp2.executeTask(fwait);
  tp2.executeTask(fwait);
  std::cerr << "free threads 1: " << tp2.hasFreeThreads() << '\n';
  tp2.executeTask(fwait);
  std::cerr << "free threads 0: " << tp2.hasFreeThreads() << '\n';

  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  std::cerr << "free threads 1: " << tp2.hasFreeThreads() << '\n';
  tp2.executeTask(fwait);
  std::cerr << "free threads 1: " << tp2.hasFreeThreads() << '\n';
  tp2.executeTask(fwait);
  tp2.executeTask(fwait);
  tp2.executeTask(fwait);
  tp2.executeTask(fwait);
  std::cerr << "free threads 0: " << tp2.hasFreeThreads() << '\n';


  std::cerr << "x: " << ::x << '\n';
}
