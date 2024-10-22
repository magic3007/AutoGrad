#include "CPPPrinter.h"

namespace Boost {

namespace Internal {

std::string CPPPrinter::print(const Group &group) {
    oss.clear();
    prefix.clear();
    suffix.clear();
    code.clear();
    group.visit_group(this);
    return code.str();
}

void CPPPrinter::visit(Ref<const Bracket> op) {
    oss << "(";
    (op->exp).visit_expr(this);
    oss << ")";
}

void CPPPrinter::visit(Ref<const IntImm> op) { oss << op->value(); }

void CPPPrinter::visit(Ref<const FloatImm> op) { oss << op->value(); }

void CPPPrinter::visit(Ref<const Binary> op) {
    (op->a).visit_expr(this);
    if (op->op_type == BinaryOpType::Add) {
        oss << " + ";
    } else if (op->op_type == BinaryOpType::Sub) {
        oss << " - ";
    } else if (op->op_type == BinaryOpType::Mul) {
        oss << " * ";
    } else if (op->op_type == BinaryOpType::Div) {
        oss << " / ";
    } else if (op->op_type == BinaryOpType::Mod) {
        oss << " % ";
    } else if (op->op_type == BinaryOpType::IntDiv) {
        oss << " / ";
    }
    (op->b).visit_expr(this);
}

void CPPPrinter::visit(Ref<const Var> op) {
    oss << op->name;
    if (op->shape.size() == 1 && op->shape[0] == 1) return;
    for (size_t i = 0; i < op->args.size(); ++i) {
        oss << "[";
        op->args[i].visit_expr(this, op->shape[i]);
        oss << "]";
    }
}

void CPPPrinter::visit(Ref<const IntImm> op, int argu) { oss << op->value(); }

void CPPPrinter::visit(Ref<const Binary> op, int argu) {
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
    if (op->op_type == BinaryOpType::Add) {
        oss << " + ";
    } else if (op->op_type == BinaryOpType::Sub) {
        oss << " - ";
    } else if (op->op_type == BinaryOpType::Mul) {
        oss << " * ";
    } else if (op->op_type == BinaryOpType::Div) {
        oss << " / ";
    } else if (op->op_type == BinaryOpType::Mod) {
        oss << " % ";
    } else if (op->op_type == BinaryOpType::IntDiv) {
        oss << " / ";
    }

    (op->b).visit_expr(this, argu);
}

void CPPPrinter::visit(Ref<const Var> op, int argu) {
    oss << op->name;
    auto r = ranges.find(op->name);
    if (r == ranges.end()) {
        indices.push_back(op->name);
        ranges[op->name] = argu;
    } else {
        int t = ranges[op->name];
        if (argu < t) ranges[op->name] = argu;
    }
}

void CPPPrinter::visit(Ref<const Move> op) {
    oss.str("");
    prefix.str("");
    suffix.str("");
    ranges.clear();
    indices.clear();
    (op->dst).visit_expr(this);
    oss << " = ";
    (op->src).visit_expr(this);
    oss << ";\n";

    for (std::string i : indices) {
        prefix_indent();
        prefix << "for (int " << i << " = 0; " << i << " < " << ranges[i]
               << "; ++" << i << ") {\n";
        enter();
    }
    prefix_indent();

    while (indent) {
        exit();
        suffix_indent();
        suffix << "}\n";
    }

    code << prefix.str() << oss.str() << suffix.str();
}

void CPPPrinter::visit(Ref<const Kernel> op) {
    for (auto stmt : op->stmt_list) {
        stmt.visit_stmt(this);
    }
}

}  // namespace Internal

}  // namespace Boost
