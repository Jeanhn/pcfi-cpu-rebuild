#include "util.hh"

int main()
{
    auto lines = readFileLines("./util_test.cc");
    for (auto l : lines)
    {
        std::cout << l << std::endl;
    }
}