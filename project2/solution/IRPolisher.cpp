#include "IRPolisher.h"

#include <utility>
#include "utils/aixlog.hpp"

Stmt IRPolisher::operator()(const Stmt &stmt, const map<string, Expr> &domains) {
  domains_ = domains;
  is_visiting_index = false;
  return mutate(stmt);
}

Expr IRPolisher::visit(Ref<const Var> op) {
  if(is_visiting_index){
    auto domain = domains_.find(op->name);
    LOG(DEBUG) << COND(domain == domains_.end()) << ": "
             << op->name << std::endl;
    // don't need to copy Index
    return domain->second;
  }else{
    is_visiting_index = true;
    auto rv = IRMutator::visit(op);
    is_visiting_index = false;
    return rv;
  }
}