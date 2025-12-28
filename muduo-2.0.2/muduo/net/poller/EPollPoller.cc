// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/poller/EPollPoller.h"

#include "muduo/base/Logging.h"
#include "muduo/net/Channel.h"

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

// On Linux, the constants of poll(2) and epoll(4)
// are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

namespace
{
const int kNew = -1;	//channel从未加入过epoll，刚创建，还没注册，epoll中没有这个fd
const int kAdded = 1;	//channel已经注册到了epoll，epoll正在监听这个fd
const int kDeleted = 2;	//channel曾经注册过，但当前已经从epoll中删除，epoll不在监听
}

EPollPoller::EPollPoller(EventLoop* loop)
  : Poller(loop),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize)
{
  if (epollfd_ < 0)
  {
    LOG_SYSFATAL << "EPollPoller::EPollPoller";
  }
}

EPollPoller::~EPollPoller()
{
  ::close(epollfd_);
}
/*
EPollPoller::poll是muduo Reactor模型中 I/O 复用的核心函数：
它调用 epoll_wait 在指定超时时间内等待事件，将发生事件的 fd
映射为对应的 Channel 并填充到 activeChannels，最后返回 poll
返回时的时间戳，供上层事件处理和定时逻辑使用
*/
Timestamp EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{	// 调用链：EventLoop::loop() -> Poller::poll() -> EPollPoller::poll()
  LOG_TRACE << "fd total count " << channels_.size();
  // ::epoll_wait成功，会返回准备好的fd的数量，或者请求超时也没有准备好的fd，返回值为0
  // ::epoll_wait失败，会返回-1和errno，表示错误类型
  int numEvents = ::epoll_wait(epollfd_,
                               &*events_.begin(),
                               static_cast<int>(events_.size()),
                               timeoutMs);
//errno不是C/C++ 关键字，它是<errno.h>库提供的标识符。
//errno - number of last error 
//The <errno.h> header file defines the integer variable errno, 
//which is set by system calls and some library functions in the event of an error to indicate what went wrong.
  int savedErrno = errno;	
  Timestamp now(Timestamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happened";
    fillActiveChannels(numEvents, activeChannels);
	// 如果::epoll_wait返回的fd数量等于传入struct epoll_event的大小，
	//则有可能是准备好的fd的数量大于struct epoll_event能够承载的数量，需要进行2倍扩容操作
    if (implicit_cast<size_t>(numEvents) == events_.size())	
    {
      events_.resize(events_.size()*2);
    }
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << "nothing happened";
  }
  else
  {
    // error happens, log uncommon ones
    if (savedErrno != EINTR)
    {
      errno = savedErrno;	//恢复errno的值为savedErrno
      LOG_SYSERR << "EPollPoller::poll()";
    }
  }
  return now;
}

void EPollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{	// 这里并没有“重新生成一个 Channel 对象”。fillActiveChannels做的事情是：
	// 把epoll返回的、已经存在的Channel* 指针收集到activeChannels容器中，仅此而已。
  assert(implicit_cast<size_t>(numEvents) <= events_.size());	//events_的容量要大于准备好的fd的数量
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
#ifndef NDEBUG
    int fd = channel->fd();
    ChannelMap::const_iterator it = channels_.find(fd);
    assert(it != channels_.end());
    assert(it->second == channel);
#endif
    channel->set_revents(events_[i].events);// 把发生的事件填到channel的revent里面
    activeChannels->push_back(channel);
  }
}

void EPollPoller::updateChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  const int index = channel->index();	//index 表示：这个Channel当前在epoll内核中的注册状态。
  LOG_TRACE << "fd = " << channel->fd()
    << " events = " << channel->events() << " index = " << index;
  if (index == kNew || index == kDeleted)
  {
    // a new one, add with EPOLL_CTL_ADD
    int fd = channel->fd();
    if (index == kNew)
    {
      assert(channels_.find(fd) == channels_.end());
      channels_[fd] = channel;
    }
    else // index == kDeleted
    {
      assert(channels_.find(fd) != channels_.end());
      assert(channels_[fd] == channel);
    }

    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else
  {
    // update existing one with EPOLL_CTL_MOD/DEL
    int fd = channel->fd();
    (void)fd;	// (void)fd; 的唯一目的：消除“未使用变量”的编译器警告，不改变任何语义。
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    assert(index == kAdded);
    if (channel->isNoneEvent())
    {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    }
    else
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void EPollPoller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  int fd = channel->fd();
  LOG_TRACE << "fd = " << fd;
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);
  assert(channel->isNoneEvent());
  int index = channel->index();
  assert(index == kAdded || index == kDeleted);
  size_t n = channels_.erase(fd);
  (void)n;
  assert(n == 1);

  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}
// epoll_event是Linux 内核定义的一个结构体，向epoll内核对象描述：①i关系哪些事件②当事件发生时，你希望内核原样带回的用户数据
void EPollPoller::update(int operation, Channel* channel)
{ 
//operation表示：这一次要对epoll中  这个fd执行ADD/MOD/DEL。
  struct epoll_event event;	
  memZero(&event, sizeof event);
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();
  LOG_TRACE << "epoll_ctl op = " << operationToString(operation)
    << " fd = " << fd << " event = { " << channel->eventsToString() << " }";
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)	
  {//When successful, epoll_ctl() returns zero.  When an error occurs, epoll_ctl() returns -1 and errno is set to indicate the error.
  /* <sys/epoll.h>头文件中定义了下面3个宏常量
  	#define EPOLL_CTL_ADD 1
	#define EPOLL_CTL_DEL 2
	#define EPOLL_CTL_MOD 3*/
    if (operation == EPOLL_CTL_DEL)
    {
      LOG_SYSERR << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
    }
    else
    {
      LOG_SYSFATAL << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
    }
  }
}

const char* EPollPoller::operationToString(int op)
{
  switch (op)
  {
    case EPOLL_CTL_ADD:
      return "ADD";
    case EPOLL_CTL_DEL:
      return "DEL";
    case EPOLL_CTL_MOD:
      return "MOD";
    default:
      assert(false && "ERROR op");
      return "Unknown Operation";
  }
}
