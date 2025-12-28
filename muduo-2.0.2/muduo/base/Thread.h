// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include "muduo/base/Atomic.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/Types.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace muduo
{

class Thread : noncopyable		// 一个Thread对象，记录了一个新线程的详细信息
{
 public:
// 线程函数的函数类型，由于该函数类型的形参为空，所以需要使用bind绑定参数
  typedef std::function<void ()> ThreadFunc;	

  explicit Thread(ThreadFunc, const string& name = string());
  // FIXME: make it movable in C++11
  ~Thread();

  void start();
  int join(); // return pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return tid_; }
  const string& name() const { return name_; }

  static int numCreated() { return numCreated_.get(); }

 private:
  void setDefaultName();

  bool       started_;
  bool       joined_;
  pthread_t  pthreadId_;	// 修改为C++11: std::shared_ptr<std::thread> thread_;
  pid_t      tid_;
  ThreadFunc func_;
  string     name_;
  CountDownLatch latch_;

  static AtomicInt32 numCreated_;
};

}  // namespace muduo
#endif  // MUDUO_BASE_THREAD_H
