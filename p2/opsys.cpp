#include "opsys.h"

void print_queue(const std::queue<Process*> &ready)
{
  std::queue<Process*> q = ready;
  std::cout << "[Q";
  if (!q.empty())
  {
    while (!q.empty())
    {
      std::cout << " " << q.front()->id;
      q.pop();
    }
  } else
  {
    std::cout << " empty";
  }
  std::cout << "]\n";
}