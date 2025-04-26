#include "/home/jean/pcfi-cpu-rebuild/util/util.hh"
#include <unordered_map>
int main()
{
    auto lines = util::readFileLines("./cora.e2f.miss_34_percent.fix.round");

    std::vector<std::string> index2name;
    std::vector<std::pair<int, int>> edges;
    for (auto &line : lines)
    {
        auto words = util::split(line);
        int idx = index2name.size();
        index2name.push_back(words.front());

        for (int i = 1; i < words.size() - 1; i++)
        {
            int v = std::stoi(words[i]);
            if (v == 1)
            {
                edges.push_back({idx, i});
            }
        }
    }

    std::vector<std::string> result;
    for (auto e : edges)
    {
        auto n1 = index2name[e.first];
        auto n2 = index2name[e.second];

        result.push_back(n1 + " " + n2);
    }

    util::writeLinesToFile("./cora.e2f.miss_34_percent.fix.round.result", result);
    return 0;
}