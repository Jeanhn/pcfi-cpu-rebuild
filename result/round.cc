#include "/home/jean/pcfi-cpu-rebuild/util/util.hh"

int main()
{
    auto fixedLines = util::readFileLines("./cora.e2f.miss_34_percent.fix");

    std::vector<std::string> roundLines;
    for (int i = 0; i < fixedLines.size(); i++)
    {
        auto features = util::split(fixedLines.at(i));

        std::vector<std::string> roundLine;
        roundLine.push_back(features.front());
        for (int j = 1; j < features.size() - 1; j++)
        {
            float value = std::stoi(features.at(j));
            if (value >= 0.5)
            {
                roundLine.push_back("1");
            }
            else
            {
                roundLine.push_back("0");
            }
        }
        roundLine.push_back(features.back());
        auto line = util::join(roundLine, ' ');
        roundLines.push_back(std::move(line));
    }

    util::writeLinesToFile("./cora.e2f.miss_34_percent.fix.round", roundLines);
    return 0;
}