#include "../CPPPrinter.h"
#include "../parser.h"

using namespace Boost::Internal;
using std::pair;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
    vector<pair<string, string> > texts = {
        {"example", "A<32, 16>[i, j] = C<32, 16>[i, j] * B<32, 16>[i, j];"},
        {"case1", "A<32, 16>[i, j] = 2;"},
        {"case4",
         "A<16, 32>[i, j] = A<16, 32>[i, j] + B<16, 32>[i, k] * C<32, 32>[k, "
         "j];"},
        {"case5",
         "A<16, 32>[i, j] = A<16, 32>[i, j] + alpha<1> * (B<16, 32>[i, k] * "
         "C<32, 32>[k, j]); A<16, 32>[i, j] = A<16, 32>[i, j] + beta<1> * "
         "D<16, 32>[i, j];"},
        {"case6",
         "A<2, 8, 5, 5>[n, k, p, q] = A<2, 8, 5, 5>[n, k, p, q] + B<2, 16, 7, "
         "7>[n, c, p + r, q + s] * C<8, 16, 3, 3>[k, c, r, s];"},
        {"case7", "B<16, 32>[i, j] = A<32, 16>[j, i];"},
        {"case10",
         "A<8, 8>[i, j] = (B<10, 10>[i, j] + B<10, 10>[i + 1, j] + B<10, 10>[i "
         "+ 2, j]) / 3;"},
        {"case_extra",
         "A<9, 9>[i, j] = (B<10, 10>[i, j] + B<10, 10>[i + 5 + j, j] + B<10, "
         "10>[i // 2, j]) / 3;"},
    };
    for (auto &e : texts) {
        auto &case_name = e.first;
        auto &text = e.second;
        fprintf(stdout, "=========================================\n");
        fprintf(stdout, "%s: %s\n", case_name.c_str(), text.c_str());
        Group kernel = parser::ParseFromString(text, 0);
        // printer
        CPPPrinter printer;
        std::cout << printer.print(kernel);
    }
    return 0;
}