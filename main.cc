#include <iostream>
#include <fstream>
#include <util.hh>
#include <controller.hh>
#include <algorithm>

void coraFix()
{
    auto edgeLines = util::readFileLines("./data/cora.cites");
    auto nodeLines = util::readFileLines("./data/cora.content");

    for (auto &line : nodeLines)
    {
        auto nodeFeatures = util::split(line, ' ');
        std::vector<std::optional<float>> features;
        for (int i = 1; i < nodeFeatures.size() - 1; i++)
        {
            features.push_back({std::stof(nodeFeatures[i])});
        }
        pcfi::Node *node = new pcfi::Node(nodeFeatures[0], features);
    }

    pcfi::Controller controller();
}

int main()
{

    return 0;
}