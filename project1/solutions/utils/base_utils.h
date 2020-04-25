//
// Created by maiji on 04/19/2020.
//

#ifndef BOOST_BASE_UTILS_H
#define BOOST_BASE_UTILS_H

#include <cstring>
#include <functional>
#include <memory>

using std::unique_ptr;

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&) = delete; \
  ClassName &operator =(const ClassName&) = delete

#define DEFER_CONCAT_IMPL(x, y) x##y
#define DEFER_CONCAT(x, y)   DEFER_CONCAT_IMPL(x, y)

class DeferredAction {
  public:
    DISALLOW_COPY_AND_ASSIGN(DeferredAction);
    explicit DeferredAction(std::function<void(void)> f): f_(std::move(f)) {}
    ~DeferredAction(){f_();}
  private:
    std::function<void(void)> f_;
};

#define DEFER(action) \
  DeferredAction DEFER_CONCAT(__deferred__, __LINE__) \
  ([&]() -> void action)

#define DCHECK(cond) \
  if (!(cond)) LOG(kLogFatal, << "DCHECK " #cond " failed!") else;

#define FATAL_CHECK(cond) \
  do{ if (!(cond)){LOG(kLogFatal, << "FATAL_CHECK " #cond " failed!");exit(-1);}} while(0)

#define LOG_ERRNO(msg) \
  LOG(kLogFatal, << #msg " " << strerror(errno))

#define RETURN_ERRNO(err) \
  do { \
    errno = err;\
    return -1;\
  } while (false)

#endif // BOOST_BASE_UTILS_H
