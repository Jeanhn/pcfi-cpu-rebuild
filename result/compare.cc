#include "/home/jean/pcfi-cpu-rebuild/util/util.hh"

int main()
{
    auto content = util::readFileLines("./cora.content");
    auto fixed = util::readFileLines("./cora.content.miss_37_percent.fix.round");

    int sum = 0;
    int count = 0;
    for (int i = 0; i < content.size(); i++)
    {
        auto lines = util::split(content.at(i));
        auto linesFixed = util::split(fixed.at(i));

        for (int j = 1; j < lines.size() - 1; j++)
        {
            float value = std::stoi(lines.at(j));
            float valueFixed = std::stoi(linesFixed.at(j));

            if (value != valueFixed)
            {
                count++;
            }
            sum++;
        }
    }
    util::debugOutput(count, sum);
    std::cout << float(count) / sum << std::endl;
    return 0;
}