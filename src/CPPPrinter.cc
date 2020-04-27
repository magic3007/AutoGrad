#include "CPPPrinter.h"

namespace Boost {

namespace Internal {

std::string CPPPrinter::print(const Expr &expr) {
    oss.clear();
    expr.visit_expr(this);
    return oss.str();
}

std::string CPPPrinter::print(const Stmt &stmt) {
    oss.clear();
    stmt.visit_stmt(this);
    return oss.str();
}

std::string CPPPrinter::print(const Group &group) {
    oss.clear();
    prefix.clear();
    suffix.clear();
    group.visit_group(this);
    return "";
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
        oss << " // ";
    }
    (op->b).visit_expr(this);
}

void CPPPrinter::visit(Ref<const Var> op) {
    oss << op->name;
    int dim = op->shape.size();
    if (dim < 2) return;
    for (size_t i = 0; i < op->args.size(); ++i) {
        oss << "[";
        // op->args[i].visit_expr(this);
        op->args[i].visit_expr(this, op->shape[i]);
        oss << "]";
    }
}

void CPPPrinter::visit(Ref<const IntImm> op, int argu) { oss << op->value(); }

void CPPPrinter::visit(Ref<const Binary> op, int argu) {
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
        oss << " // ";
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
               << "; ++i) {\n";
        enter();
    }
    prefix_indent();
    std::cout << prefix.str();
    
    std::cout << oss.str();

    while (indent) {
        exit();
        suffix_indent();
        suffix << "}\n";
    }
    std::cout << suffix.str();
}

void CPPPrinter::visit(Ref<const Kernel> op) {
    for (auto stmt : op->stmt_list) {
        stmt.visit_stmt(this);
    }
}

}  // namespace Internal

}  // namespace Boost
