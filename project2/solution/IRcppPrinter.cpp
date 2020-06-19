#include "IRcppPrinter.h"

std::string IRcppPrinter::print(const Group &group) {
  oss.str("");
  group.visit_group(this);
  return oss.str();
}

void IRcppPrinter::visit(Ref<const IntImm> op) { oss << op->value(); }

void IRcppPrinter::visit(Ref<const UIntImm> op) { oss << op->value(); }

void IRcppPrinter::visit(Ref<const FloatImm> op) { oss << op->value(); }

void IRcppPrinter::visit(Ref<const Unary> op) {
  if (op->op_type == UnaryOpType::Neg) {
    oss << "-";
  } else if (op->op_type == UnaryOpType::Not) {
    oss << "!";
  }
  (op->a).visit_expr(this);
}

void IRcppPrinter::visit(Ref<const Binary> op) {
  oss << "(";
  (op->a).visit_expr(this);
  if (op->op_type == BinaryOpType::Add) {
    oss << " + ";
  } else if (op->op_type == BinaryOpType::Sub) {
    oss << " - ";
  } else if (op->op_type == BinaryOpType::Mul) {
    oss << " * ";
  } else if (op->op_type == BinaryOpType::Div) {
    oss << " / ";
  } else if (op->op_type ==
             BinaryOpType::IntDiv) { // Integer division is "/" in C
    oss << " / ";
  } else if (op->op_type == BinaryOpType::Mod) {
    oss << " % ";
  } else if (op->op_type == BinaryOpType::And) {
    oss << " && ";
  } else if (op->op_type == BinaryOpType::Or) {
    oss << " || ";
  }
  (op->b).visit_expr(this);
  oss << ")";
}

void IRcppPrinter::visit(Ref<const Compare> op) {
  (op->a).visit_expr(this);
  if (op->op_type == CompareOpType::LT) {
    oss << " < ";
  } else if (op->op_type == CompareOpType::LE) {
    oss << " <= ";
  } else if (op->op_type == CompareOpType::EQ) {
    oss << " == ";
  } else if (op->op_type == CompareOpType::GE) {
    oss << " >= ";
  } else if (op->op_type == CompareOpType::GT) {
    oss << " > ";
  } else if (op->op_type == CompareOpType::NE) {
    oss << " != ";
  }
  (op->b).visit_expr(this);
}

void IRcppPrinter::visit(Ref<const Select> op) {
  (op->cond).visit_expr(this);
  oss << " ? ";
  (op->true_value).visit_expr(this);
  oss << " : ";
  (op->false_value).visit_expr(this);
}

void IRcppPrinter::visit(Ref<const Bracket> op) {
  oss << "(";
  (op->exp).visit_expr(this);
  oss << ")";
}

void IRcppPrinter::visit(Ref<const Var> op) {
  oss << op->name;
  for (const auto &arg : op->args) {
    oss << "[";
    arg.visit_expr(this);
    oss << "]";
  }
}

void IRcppPrinter::visit(Ref<const Index> op) { oss << op->name; }

void IRcppPrinter::visit(Ref<const LoopNest> op) {
  for (const auto &index : op->index_list) {
    print_indent();
    auto name = index.as<Index>()->name;
    auto dom = index.as<Index>()->dom.as<Dom>();
    int begin = dom->begin.as<IntImm>()->value();
    int end = begin + dom->extent.as<IntImm>()->value();
    oss << "for (int " << name << " = " << begin << "; " << name << " < " << end
        << "; " << name << "++){\n";
    enter();
  }
  for (const auto &body : op->body_list) {
    body.visit_stmt(this);
  }
  for (auto index : op->index_list) {
    exit();
    print_indent();
    oss << "}\n";
  }
}

void IRcppPrinter::visit(Ref<const Move> op) {
  print_indent();
  (op->dst).visit_expr(this);
  oss << move_stmt_operator_;
  (op->src).visit_expr(this);
  oss << ";\n";
}

void IRcppPrinter::visit(Ref<const Kernel> op) {
  print_indent();
  enter();
  move_stmt_operator_ = " = ";
  for (const auto &stmt : op->stmt_list) {
    stmt.visit_stmt(this);
    move_stmt_operator_ = " += ";
  }
  exit();
}
