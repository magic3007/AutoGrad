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

class DeferAction {
  public:
    DISALLOW_COPY_AND_ASSIGN(DeferAction);
    explicit DeferAction(std::function<void(void)> f): f_(std::move(f)) {}
    ~DeferAction(){f_();}
  private:
    std::function<void(void)> f_;
};

#endif // BOOST_BASE_UTILS_H
