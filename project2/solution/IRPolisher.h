#ifndef BOOST_IRPOLISHER_H
#define BOOST_IRPOLISHER_H

#include "IRMutator.h"
#include <string>
#include <map>

using namespace Boost::Internal;
using std::string;
using std::map;

class IRPolisher : public IRMutator{
public:
  Stmt operator ()(const Stmt &stmt, const map<string, Expr> &domains);
protected:
  virtual Expr visit(Ref<const Var>) override;
private:
  bool is_visiting_index;
  map<string, Expr> domains_;
};

#endif // BOOST_IRPOLISHER_H
