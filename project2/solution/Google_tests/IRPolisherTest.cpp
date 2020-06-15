#include "gtest/gtest.h"
#include "BaseTest.h"
#include "IndexAnalyst.h"
#include "IRPolisher.h"
#include "IRPrinter.h"
#include "utils/aixlog.hpp"
#include <vector>

namespace{

using namespace base_test;
using namespace Boost::Internal;
using std::vector;

class IRPolisherTest : public BaseTest{
};

TEST_P(IRPolisherTest, DisplayInferredDomains){
  IndexAnalyst index_analyzer;
  auto domains = index_analyzer(main_stmt_);
  std::cout<<"=================================" << std::endl;
  std::cout<< text_ << std::endl;
  for(auto &domain : domains){
    auto index = domain.second.as<Index>();
    auto dom = index->dom.as<Dom>();
    auto index_type_str = index->index_type == IndexType::Spatial
        ? "Spatial" : "Reduce";
    auto begin = dom->begin.as<IntImm>()->value();
    auto extent = dom->extent.as<IntImm>()->value();
    auto end = begin + extent;
    std::cout << index->name << std::endl <<
        " index type: " << index_type_str << std::endl <<
        " domain: " << "[" << begin <<", " << end << ")" << std::endl;
  }
}

TEST_P(IRPolisherTest, DisplayPolishedAST){
  AixLog::Log::init<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
  IndexAnalyst index_analyzer;
  auto domains = index_analyzer(main_stmt_);
  std::cout<<"=================================" << std::endl;
  std::cout<< text_ << std::endl << std::endl;
  IRPolisher polisher;
  auto polished_main_stmt = polisher(main_stmt_, domains);
  vector<Expr> index_list{};
  index_list.reserve(domains.size());
  for(auto &iter : domains){
    index_list.push_back(iter.second);
  }
  Stmt loop_nest = LoopNest::make(index_list, {polished_main_stmt});
  IRPrinter printer;
  std::cout << printer.print(loop_nest) << std::endl;
}

INSTANTIATE_TEST_SUITE_P(OnTheFlyIRPolisherTest,IRPolisherTest,
                         Values(
                             &get_case1_text,
                             &get_case2_text,
                             &get_case3_text,
                             &get_case4_text,
                             &get_case5_text,
                             &get_case6_text,
                             &get_case7_text,
                             &get_case8_text,
                             &get_case9_text,
                             &get_case10_text
));

}

