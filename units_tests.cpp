#include <gtest/gtest.h>

#include "ConcurrentQueue.hpp"
#include <random>
#include <thread>

void pushInThread(ConcurrentQueue<int> *queue){
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  int intput = 2;
  queue->push(intput);
}

/*
  Test functions when ConcurrentQueue is empty 
*/
TEST(ConcurrentQueue, emptyQueue){
  ConcurrentQueue<int> queue;

  // queue is empty
  int intput = 1;
  ASSERT_EQ(queue.try_pop(intput), false);
  ASSERT_EQ(queue.size(), 0);
  ASSERT_EQ(queue.empty(), true);

  // Wait and pop
  // We create 1 thread to push after 300 ms to wait.
  // wait_and_pop function wait until the thread to push is finish
  int output = 0;
  std::thread t1(pushInThread, &queue);
  t1.detach();
  queue.wait_and_pop(output);
  ASSERT_EQ(output, 2);
  ASSERT_EQ(queue.try_pop(output), false);

  // Simple push and try_pop in empty queue
  ASSERT_EQ(queue.size(), 0);
  output = 0;
  queue.push(intput);
  ASSERT_EQ(queue.try_pop(output), true);
  ASSERT_EQ(output, 1);
}

void wait_and_popInThread(ConcurrentQueue<int> *queue){
  int output = 0;
  queue->wait_and_pop(output);
}

void try_popInThread(ConcurrentQueue<int> *queue){
  int output = 0;
  queue->try_pop(output);
}

/*
  Test functions when ConcurrentQueue is filled
*/
TEST(ConcurrentQueue, filledQueue){
  ConcurrentQueue<int> queue;

  //-------------
  // wait_and_pop
  //-------------

  const size_t nbThread = 10;

  // Fill queue
  for (size_t i = 0; i < nbThread; ++i)
  {
    queue.push(i);
  }
  ASSERT_EQ(queue.size(), 10);
  ASSERT_EQ(queue.empty(), false);

  // Create thread
  for (size_t i = 0; i < nbThread; ++i)
  {
    std::thread t1(wait_and_popInThread, &queue);
    t1.join();
  }

  ASSERT_EQ(queue.size(), 0);
  ASSERT_EQ(queue.empty(), true);
  int output = 0;
  ASSERT_EQ(queue.try_pop(output), false);

  //-------------
  // try_pop
  //-------------

  // Fill queue
  for (size_t i = 0; i < nbThread; ++i)
  {
    queue.push(i);
  }
  ASSERT_EQ(queue.size(), 10);
  ASSERT_EQ(queue.empty(), false);

  // Create thread
  for (size_t i = 0; i < nbThread; ++i)
  {
    std::thread t1(try_popInThread, &queue);
    t1.join();
  }

  ASSERT_EQ(queue.size(), 0);
  ASSERT_EQ(queue.empty(), true);
  output = 0;
  ASSERT_EQ(queue.try_pop(output), false);
}

/*
  Various tests
*/
TEST(ConcurrentQueue, VariousTests){

  ConcurrentQueue<int> queue;

  // Differents push
  int input = 1;
  int & inputRef = input;
  const int inputConst = 2;
  queue.push(input);
  queue.push(inputRef);
  queue.push(inputConst);
  queue.push(10);
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
