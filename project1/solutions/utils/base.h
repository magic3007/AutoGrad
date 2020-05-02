//
// Created by maiji on 05/02/2020.
//

#ifndef BOOST_BASE_H
#define BOOST_BASE_H

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&) = delete; \
  ClassName& operator =(const ClassName&) = delete

#define CONCAT_DEFER_IMPL(x,y) x##y
#define CONCAT_DEFER(x,y) CONCAT_DEFER_IMPL(x,y)

class DeferredAction{
public:
    DISALLOW_COPY_AND_ASSIGN(DeferredAction);
    explicit DeferredAction(std::function<void(void)> f) :
      f_(std::move(f)){}
    inline ~DeferredAction(){
      f_();
    }
private:
    std::function<void(void)> f_;
  };

#define DEFER(action) \
  DeferredAction CONCAT_DEFER(__deferred_, __LINE__) \
  ([&]() -> void action)

#endif // BOOST_BASE_H
