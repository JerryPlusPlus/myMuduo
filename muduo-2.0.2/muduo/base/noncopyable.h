#ifndef MUDUO_BASE_NONCOPYABLE_H
#define MUDUO_BASE_NONCOPYABLE_H

namespace muduo
{

class noncopyable	//该类不可拷贝、不可赋值，以该类为基类的父类也不可拷贝不可赋值
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}  // namespace muduo

#endif  // MUDUO_BASE_NONCOPYABLE_H
