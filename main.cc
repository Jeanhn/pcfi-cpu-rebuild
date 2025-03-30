#include <iostream>
#include <fstream>
#include <util.hh>
#include <controller.hh>
#include <algorithm>

void coraFix()
{
    auto nodeLines = util::readFileLines("/home/jean/pcfi-cpu-rebuild/test_data/cora_test/cora_test.content");
    std::vector<pcfi::Node *> nodes;
    for (auto &line : nodeLines)
    {
        auto nodeFeatures = util::split(line, ' ');
        std::vector<std::optional<float>> features;
        for (int i = 1; i < nodeFeatures.size() - 1; i++)
        {
            try
            {
                features.push_back({std::stof(nodeFeatures[i])});
            }
            catch (std::exception e)
            {
                features.push_back(std::optional<float>());
            }
        }
        pcfi::Node *node = new pcfi::Node(nodeFeatures[0], features);
        nodes.push_back(node);
    }

    auto edgeLines = util::readFileLines("/home/jean/pcfi-cpu-rebuild/test_data/cora_test/cora_test.cites");
    std::vector<std::pair<std::string, std::string>> edges;
    for (auto &line : edgeLines)
    {
        auto points = util::split(line, ' ');
        std::pair<std::string, std::string> edge{points[0], points[1]};
        edges.push_back(edge);
    }

    pcfi::Controller controller(std::move(nodes), std::move(edges));

    controller.iterate(2);
}

int main()
{
    coraFix();
    return 0;
}