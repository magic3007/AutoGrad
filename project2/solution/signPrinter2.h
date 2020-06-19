#ifndef BOOST_signPrinter2_H
#define BOOST_signPrinter2_H

#include <map>
#include <vector>
#include <array>
#include <string>

#include "IRPrinter.h"

namespace Boost {

namespace Internal {

class signPrinter2 : public IRPrinter {
   public:
    signPrinter2() : IRPrinter(){}
    signPrinter2(std::vector<std::string> _ins, std::vector<std::string> _outs, std::string _type, std::vector<std::string> _grad) : IRPrinter() 
    {
        ins = _ins;outs=_outs;type=_type;
        grad = _grad;
    }
    std::string print(const Group &);


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
    std::map<std::string, std::string> ranges;
    std::vector<std::string> ins;
    std::vector<std::string> outs;
    std::vector<std::string> grad;
    std::string type;
};

}  // namespace Internal

}  // namespace Boost

#endif  