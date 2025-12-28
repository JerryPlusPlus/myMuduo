// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include "muduo/base/Types.h"

namespace muduo
{
namespace CurrentThread
{	
//声明一个线程局部（Thread-Local Storage, TLS）的全局整型变量,每个线程都有自己独立的一份副本，互不干扰。
  extern __thread int t_cachedTid;	// __thread相当于thread_local
  extern __thread char t_tidString[32];
  extern __thread int t_tidStringLength;
  extern __thread const char* t_threadName;
  void cacheTid();	// TID的访问是一个系统调用，总是从用户空间切换到内核空间比较消耗事件，所以将TID缓存起来

  inline int tid()
  {	//判断当前线程的TID是否访问过，没访问过，则调用cacheTid()记录TID，访问过则直接返回缓存中的TID记录
    if (__builtin_expect(t_cachedTid == 0, 0))	
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();

  void sleepUsec(int64_t usec);  // for testing

  string stackTrace(bool demangle);
}  // namespace CurrentThread
}  // namespace muduo

#endif  // MUDUO_BASE_CURRENTTHREAD_H
