#ifndef BOOST_CPPPRINTER_H
#define BOOST_CPPPRINTER_H

#include <map>
#include <vector>

#include "IRPrinter.h"

namespace Boost {

namespace Internal {

class CPPPrinter : public IRPrinter {
   public:
    CPPPrinter() : IRPrinter() {}
    std::string print(const Group &);

    void prefix_indent() {
        for (int i = 0; i < indent; ++i) prefix << " ";
    }

    void suffix_indent() {
        for (int i = 0; i < indent; ++i) suffix << " ";
    }

    void visit(Ref<const IntImm>) override;
    void visit(Ref<const FloatImm>) override;
    void visit(Ref<const Binary>) override;
    void visit(Ref<const Var>) override;
    void visit(Ref<const Move>) override;
    void visit(Ref<const Kernel>) override;
    void visit(Ref<const Bracket>) override;

    void visit(Ref<const IntImm>, int) override;
    void visit(Ref<const Binary>, int) override;
    void visit(Ref<const Var>, int) override;

   private:
    std::map<std::string, int> ranges;
    std::vector<std::string> indices;
    std::ostringstream prefix, suffix;
    // std::vector<std::string> statements;
};

}  // namespace Internal

}  // namespace Boost

#endif  // BOOST_CPPPRINTER_H