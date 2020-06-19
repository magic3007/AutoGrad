
#ifndef BOOST_IRCPPPRINTER_H
#define BOOST_IRCPPPRINTER_H

#include "IR.h"
#include <string>
#include "IRPrinter.h"

// ================================================
// healper functions
// ================================================

using namespace Boost::Internal;
using std::string;
using std::stack;
using std::vector;
using std::map;

class IRcppPrinter : public IRPrinter {
   public:
    IRcppPrinter() : IRPrinter() {}
    std::string print(const Group &);

    void prefix_indent() {
        for (int i = 0; i < indent; ++i) prefix << " ";
    }

    void suffix_indent() {
        for (int i = 0; i < indent; ++i) suffix << " ";
    }

    void enter() { indent += 2; }

    void exit() { indent -= 2; }

    void visit(Ref<const IntImm>) override;
    void visit(Ref<const UIntImm>) override;
    void visit(Ref<const FloatImm>) override;
    void visit(Ref<const StringImm>) override;
    void visit(Ref<const Unary>) override;
    void visit(Ref<const Binary>) override;
    void visit(Ref<const Select>) override;
    void visit(Ref<const Compare>) override;
    void visit(Ref<const Call>) override;
    void visit(Ref<const Var>) override;
    void visit(Ref<const Cast>) override;
    void visit(Ref<const Ramp>) override;
    void visit(Ref<const Index>) override;
    void visit(Ref<const Dom>) override;
    void visit(Ref<const Bracket>) override;
    void visit(Ref<const LoopNest>) override;
    void visit(Ref<const IfThenElse>) override;
    void visit(Ref<const Move>) override;
    void visit(Ref<const Kernel>) override;

    void visit(Ref<const IntImm>, int) override;
    void visit(Ref<const Binary>, int) override;
    void visit(Ref<const Var>, int) override;

   protected:
    std::ostringstream oss;
    int indent;
    bool print_range;
    bool print_arg;
};


#endif  // BOOST_IRCPPPRINTER_H