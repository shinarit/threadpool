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

int main()
{
  X x{10};
  std::function<void()> f1 = std::bind(alma);
  std::function<void()> f2 = std::bind(korte, 2);
  std::function<void()> f3 = std::bind(&X::alma, &x);
  std::function<void()> f4 = std::bind(&X::korte, &x, 2);

  ThreadPool tp(5);
  tp.executeTask(f1);
  tp.executeTask(f2);
  tp.executeTask(f3);
  tp.executeTask(f4);
  
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  tp.executeTask(f4);
  tp.executeTask(f3);
  tp.executeTask(f2);
  tp.executeTask(f1);
  
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::cerr << "x: " << ::x << '\n';
}
