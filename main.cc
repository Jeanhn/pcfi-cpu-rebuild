#include <iostream>
#include <fstream>
#include <util.hh>
#include <controller.hh>
#include <algorithm>

pcfi::Controller readCora(float alpha, int round, const std::string &nodeSource, const std::string &edgeSource)
{
    auto nodeLines = util::readFileLines(nodeSource);
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

    auto edgeLines = util::readFileLines(edgeSource);
    std::vector<std::pair<std::string, std::string>> edges;
    for (auto &line : edgeLines)
    {
        auto points = util::split(line, ' ');
        std::pair<std::string, std::string> edge{points[0], points[1]};
        edges.push_back(edge);
    }

    pcfi::Controller controller(std::move(nodes), std::move(edges), alpha, round);
    return controller;
}

void coraFix(float alpha, int round, const std::string &nodeSource, const std::string &edgeSource, const std::string &fixTarget)
{
    auto controller = readCora(alpha, round, nodeSource, edgeSource);

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

    controller.saveTo(fixTarget, false);
}

void coraRandomGenerate(const std::string &nodeSource, const std::string &edgeSource, const std::string &missingTaget)
{
    auto controller = readCora(0, 0, nodeSource, edgeSource);
    controller.generateRandomMissingFeatures(5);
    controller.saveTo(missingTaget, true);
}

void startCora()
{
    int mode;
    float alpha;
    int round;
    std::string nodeSource, edgeSource, fixTarget, missingTarget;

    auto configLines = util::readFileLines("/home/jean/pcfi-cpu-rebuild/config.txt");
    for (auto l : configLines)
    {
        auto kv = util::split(l, ':');
        if (kv[0] == "mode")
        {
            mode = std::stoi(kv[1]);
            continue;
        }

        if (mode == 0)
        {
            if (kv[0] == "alpha")
            {
                alpha = std::stof(kv[1]);
            }
            else if (kv[0] == "round")
            {
                round = std::stoi(kv[1]);
            }
            else if (kv[0] == "nodeSource")
            {
                nodeSource = kv[1];
            }
            else if (kv[0] == "edgeSource")
            {
                edgeSource = kv[1];
            }
            else if (kv[0] == "fixTarget")
            {
                fixTarget = kv[1];
            }
        }
        else
        {
            if (kv[0] == "nodeSource")
            {
                nodeSource = kv[1];
            }
            else if (kv[0] == "edgeSource")
            {
                edgeSource = kv[1];
            }
            else if (kv[0] == "missingTarget")
            {
                missingTarget = kv[1];
            }
        }
    }

    if (mode == 1)
    {
        coraRandomGenerate(nodeSource, edgeSource, missingTarget);
        return;
    }
    float alpha;
    int round;
    coraFix(alpha, round, nodeSource, edgeSource, fixTarget);
}

int main()
{

    return 0;
}