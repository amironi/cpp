
#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std;

enum State { Pending, Running, Completed };

class Task {
public:
  virtual void run() = 0;

  State state;
};

class ThreadPool {
public:
  ThreadPool(int num_threads) : m_stop(false) {
    for (int i = 0; i < num_threads; i++) {
      m_threads.emplace_back(&ThreadPool::execute, this);
    }
  }

  ~ThreadPool() {
    stop();
    join();
  }

  void stop() {
    m_stop = true;
    m_condition.notify_all();
  }

  void join() {
    for (auto &thread : m_threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  void execute() {
    while (!m_stop) {
      unique_lock<mutex> lock(m_mutex);
      m_condition.wait(lock, [this] { return m_stop || !m_list.empty(); });

      if (m_stop && m_list.empty()) {
        continue;
      }

      Task *task = m_list.front();
      m_list.pop_front();
      lock.unlock();

      task->state = Running;
      task->run();
      task->state = Completed;
      m_condition.notify_one();
    }
  }

  void add_task(Task *task) {
    {
      unique_lock<mutex> lock(m_mutex);
      m_list.push_back(task);
    }

    m_condition.notify_one();
  }

private:
  vector<thread> m_threads;
  mutex m_mutex;
  condition_variable m_condition;
  list<Task *> m_list;
  atomic<bool> m_stop;
};

int main() {

  printf("Test\n");

  return 0;
}
