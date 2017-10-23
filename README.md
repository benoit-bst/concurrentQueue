https://travis-ci.org/benoit-bst/concurrentQueue.svg?branch=master

# Introduction

ConcurrentQueue class is a thread safe queue. This is an implementation of the classic multiple producer, multiple consumer thread-safe concept. You don't need to care of thread's synchronization. ConcurrentQueue class is non-copyable.

Using of C++11

Methods are :

* size_t **size()** *Return queue size*
* bool **empty()** *Return* **true** *is queue is empty*
* void **push(T elem)** *Push new element in queue*
* bool **try_pop(T & elem)** *Try to pop element, return* **true** *if queue is not empty*
* void **wait_and_pop(T & elem)**  *Wait if queue is empty before to pop element*

## Examples

```c++

  // Create cuncurrent queue
  ConcurrentQueue<int> queue;

  // Push new element
  queue.push(10);

  // size of queue
  int size = queue.size(); // size = 1

  // Simple try pop
  // If internal queue is empty, we don't
  // pop element and return false.
  // This function is not blocking.
  int output = 0;
  bool sucess = queue.try_pop(output); // sucess = true
  bool sucess = queue.try_pop(output); // sucess = false because size = 0

  // Push new element
  queue.push(11);

  // Wait and pop
  // if queue is empty, wait_and_pop wait an new element before to pop.
  // This function is blocking.
  queue.wait_and_pop(output);

  // if queue is empty
  bool isEmpty = queue.empty(); // isEmpty = true

```

## Environment

Required tools et library :

* CMake >= 3.5.1
* gcc >= 5.4.0

debian installation :
```shell
$ sudo apt-get install cmake gcc
```

## Source recovery

```
git clone https://github.com/benoit-bst/concurrentQueue
```

## Compilation

```shell
# Create project_root/build
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Running tests

```shell
# Inside project_root/tests
$ ./units_tests
```
