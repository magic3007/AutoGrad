#ifndef BOOST_IRCPPPRINTER_H
#define BOOST_IRCPPPRINTER_H

#include "IR.h"
#include <string>
#include "IRPrinter.h"

using namespace Boost::Internal;
using std::string;

class IRcppPrinter : public IRPrinter {
   public:
    string print(const Group &) override;

    void visit(Ref<const IntImm>) override;
    void visit(Ref<const UIntImm>) override;
    void visit(Ref<const FloatImm>) override;
    void visit(Ref<const Unary>) override;
    void visit(Ref<const Binary>) override;
    void visit(Ref<const Select>) override;
    void visit(Ref<const Compare>) override;
    void visit(Ref<const Var>) override;
    void visit(Ref<const Index>) override;
    void visit(Ref<const Bracket>) override;
    void visit(Ref<const LoopNest>) override;
    void visit(Ref<const Move>) override;
    void visit(Ref<const Kernel>) override;
  private:
    string move_stmt_operator_;
};


#endif  // BOOST_IRCPPPRINTER_H