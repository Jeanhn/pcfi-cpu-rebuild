#include <iostream>
#include <fstream>
#include <util.hh>
#include <controller.hh>
#include <algorithm>

void coraFix(float alpha, int round)
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
        pcfi::Node *node = new pcfi::Node(std::move(nodeFeatures[0]), std::move(features), std::move(nodeFeatures.back()));
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

    pcfi::Controller controller(std::move(nodes), std::move(edges), alpha, round);

    std::vector<std::vector<float>> featuresAfterFix;
    featuresAfterFix.reserve(controller.featureSize());
    for (int i = 0; i < controller.featureSize(); i++)
    {
        auto f = controller.iterate(i);
        featuresAfterFix.push_back(std::move(f));
    }

    pcfi::Matrix m(std::move(featuresAfterFix));
    m.Reserve();
    featuresAfterFix = std::move(m.exportData());

    controller.fixNodes(std::move(featuresAfterFix));

    controller.saveTo("/home/jean/pcfi-cpu-rebuild/test_data/cora_test/cora_test.content.fixed");
}

int main(int argc, char *argv[])
{
    float alpha;
    int round;
    alpha = atof(argv[1]);
    round = atoi(argv[2]);
    coraFix(alpha, round);
    return 0;
}