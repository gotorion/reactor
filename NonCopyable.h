#ifndef __NONCOPYABLE_H_
#define __NONCOPYABLE_H_

class NonCopyable {
 public:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;
};

#endif
