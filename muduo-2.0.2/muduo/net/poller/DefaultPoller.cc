// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/Poller.h"
#include "muduo/net/poller/PollPoller.h"
#include "muduo/net/poller/EPollPoller.h"

#include <stdlib.h>

using namespace muduo::net;
// EventLoop可以通过该接口获取默认的IO复用的具体实现
Poller* Poller::newDefaultPoller(EventLoop* loop)
{	// newDefaultPoller是Poller抽象类的方法，但是返回的是子类对象EPollPoller/PollPoller
	//但是想要返回子类对象就必须现有子类的定义，于是在另一个.cc文件中进行newDefaultPoller函数的定义
  if (::getenv("MUDUO_USE_POLL"))
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
