#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
 
/**
 * @brief ConcurrentQueue class
 * 
 * @details This class is a thread safe queue.
 * This is an implementation of the classic multiple producer, 
 * multiple consumer thread-safe concept.
 * You don't need to care of thread's synchronization.
 * 
 * @note ConcurrentQueue can be very useful to design thread pool.
 * @warning ConcurrentQueue class is non-copyable
 */
template <typename T>
class ConcurrentQueue
{
public:

  ConcurrentQueue() = default;

  ConcurrentQueue (const ConcurrentQueue &) = delete;
  ConcurrentQueue & operator = (const ConcurrentQueue &) = delete;

  ConcurrentQueue(ConcurrentQueue&& other) = delete;
  ConcurrentQueue& operator=(ConcurrentQueue&& other) = delete;
 
  bool try_pop(T& popped_value);
  void wait_and_pop(T& item);

  void push(const T& item);
  void push(T&& item);

  size_t size();
  bool empty();

private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

/**
 * @brief Simple try and pop.
 * If internal queue is empty, we don't
 * pop element and return false.
 * This function is not blocking.
 * 
 * @param Popped element
 * @return If data is popped return true, else false
 */
template <typename T>
bool ConcurrentQueue<T>::try_pop(T& popped_value)
{
  std::unique_lock<std::mutex> mlock(mutex_);
  if (queue_.empty()){
    return false;
  }
  popped_value = queue_.front();
  queue_.pop();
  return true;
}

/**
 * @brief Pop element
 * If internal queue is empty, we wait
 * new element to pop them.
 * This function is blocking.
 * 
 * @param Popped element
 */
template <typename T>
void ConcurrentQueue<T>::wait_and_pop(T& item)
{
  std::unique_lock<std::mutex> mlock(mutex_);
  while (queue_.empty()){
    cond_.wait(mlock);
  }
  item = queue_.front();
  queue_.pop();
}

/**
 * @brief Push new element
 */
template <typename T>
void ConcurrentQueue<T>::push(const T& item)
{
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push(item);
  mlock.unlock();
  cond_.notify_one();
}

/**
 * @brief Push new element.
 * @note Overload for move semantic
 */
template <typename T>
void ConcurrentQueue<T>::push(T&& item)
{
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push(std::move(item));
  mlock.unlock();
  cond_.notify_one();
}

/**
 * @return Concurrent queue size
 */
template <typename T>
inline size_t ConcurrentQueue<T>::size()
{
  std::unique_lock<std::mutex> mlock(mutex_);
  return queue_.size();
}

/**
 * @return True if ConcurrentQueue is empty, else false
 */
template <typename T>
inline bool ConcurrentQueue<T>::empty()
{ 
  std::unique_lock<std::mutex> mlock(mutex_);
  return queue_.empty();
}
