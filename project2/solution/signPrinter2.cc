#include "signPrinter2.h"

namespace Boost {

namespace Internal {

std::string signPrinter2::print(const Group &group) {
	ranges.clear();
    group.visit_group(this);
    std::string ret = "(";
    bool first = 1;
    for (size_t i = 0; i < ins.size(); ++ i)
    {
        bool flag = false;
        for (size_t j = 0; j < grad.size() && !flag; ++ j)
            if (ins[i] != grad[j]) flag = true;
        if (!flag) continue;
        std::string name = ins[i];
        std::string size = ranges[name];
        if (!first)
            ret += ", ";
        //ret += "float ";
        ret += type + " ";
        first = 0;
        if (size.length() == 0)
            ret += "&" +name;
        else
            ret += "(&"+name+")"+size;
    }
    for (size_t i = 0; i < outs.size(); ++ i)
    {
        std::string name = outs[i];
        std::string size = ranges[name];
        if (!first)
            ret += ", ";
        //ret += "float ";
        ret += type + " ";
        first = 0;
        if (size.length() == 0)
            ret += "&d" +name;
        else
            ret += "(&d"+name+")"+size;   
    }
    for (size_t i = 0; i < grad.size(); ++ i)
    {
        std::string name = grad[i];
        std::string size = ranges[name];
        if (!first)
            ret += ", ";
        //ret += "float ";
        ret += type + " ";
        first = 0;
        if (size.length() == 0)
            ret += "&d" +name;
        else
            ret += "(&d"+name+")"+size;   
    }
    ret += ")";
    return ret;
}

void signPrinter2::visit(Ref<const Bracket> op) {
    (op->exp).visit_expr(this);
}

void signPrinter2::visit(Ref<const IntImm> op) { }

void signPrinter2::visit(Ref<const FloatImm> op) {  }

void signPrinter2::visit(Ref<const Binary> op) {
    (op->a).visit_expr(this);
    (op->b).visit_expr(this);
}

void signPrinter2::visit(Ref<const Var> op) {
    std::string name = op->name;
    if (op->shape.size() == 1 && op->shape[0] == 1) 
    {
        ranges[name] = "";
        return;
    }
    std::string size = "[";

    for (size_t j = 0; j < op->shape.size(); ++ j) {
        size = size + std::to_string(op->shape[j]);
        if (j < op->shape.size() - 1) {
            size = size +  "][";
        }
    }
    size += "]";
    ranges[name] = size;
}

void signPrinter2::visit(Ref<const IntImm> op, int argu) {}

void signPrinter2::visit(Ref<const Binary> op, int argu) {
    // right side is an integer immediate
    auto p = std::dynamic_pointer_cast<const IntImm>(op->b.real_ptr());
    if (p != nullptr) {
        if (op->op_type == BinaryOpType::Add) {
            argu -= p->value();
        } else if (op->op_type == BinaryOpType::Mul) {
            argu /= p->value();
        } else if (op->op_type == BinaryOpType::IntDiv) {
            argu *= p->value();
        }
    }
    (op->a).visit_expr(this, argu);

    (op->b).visit_expr(this, argu);
}

void signPrinter2::visit(Ref<const Var> op, int argu) {
}

void signPrinter2::visit(Ref<const Move> op) {
   
    (op->dst).visit_expr(this);
    (op->src).visit_expr(this);
}

void signPrinter2::visit(Ref<const Kernel> op) {
    for (auto stmt : op->stmt_list) {
        stmt.visit_stmt(this);
    }
}

}  // namespace Internal

}  // namespace Boost