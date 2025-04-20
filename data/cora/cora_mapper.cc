#include "/home/jean/pcfi-cpu-rebuild/util/util.hh"
#include <unordered_map>

void initEdgeFeature()
{
    auto featureLines = util::readFileLines("./cora.content");
    std::vector<std::string> nodes;
    std::unordered_map<std::string, int> index;
    for (auto cite : featureLines)
    {
        auto art = util::split(cite);
        nodes.push_back(art[0]);
        index.insert({art[0], nodes.size() - 1});
    }

    std::vector<std::vector<int>> nodesFeatures;
    nodesFeatures.resize(nodes.size());
    for (int i = 0; i < nodesFeatures.size(); i++)
    {
        auto &nodeFeatures = nodesFeatures[i];
        nodeFeatures.resize(nodes.size());
        nodeFeatures[i] = 1;
    }

    auto edges = util::readFileLines("./cora.cites");
    for (auto edge : edges)
    {
        auto v = util::split(edge);
        int idx1 = index[v[0]];
        int idx2 = index[v[1]];
        nodesFeatures[idx1][idx2] = nodesFeatures[idx2][idx1] = 1;
    }

    std::vector<std::string> resultLines;
    int i = 0;
    for (auto &nodeFeatures : nodesFeatures)
    {
        std::vector<std::string> line;
        line.push_back(nodes[i]);
        for (int f : nodeFeatures)
        {
            line.push_back(std::to_string(f));
        }
        line.push_back("edge");

        auto lineStr = util::join(line, ' ');
        resultLines.push_back(std::move(lineStr));
        i++;
    }

    util::writeLinesToFile("./cora.e2f", resultLines);
}

void calculateCosine()
{
    auto featureLines = util::readFileLines("./cora.content");
    std::vector<std::string> lines;

    for (int i = 0; i < featureLines.size(); i++)
    {
        std::cout << "start calculate: " << i << std::endl;
        std::vector<std::string> line;
        for (int j = i; j < featureLines.size(); j++)
        {
            auto lineI = util::split(featureLines[i]);
            auto lineJ = util::split(featureLines[j]);

            std::vector<int> vecI, vecJ;
            for (int k = 1; k < lineI.size() - 1; k++)
            {
                int numI = std::stoi(lineI[k]);
                int numJ = std::stoi(lineJ[k]);

                vecI.push_back(numI);
                vecJ.push_back(numJ);
            }

            float cosValue = util::cosine(vecI, vecJ);
            line.push_back(std::to_string(cosValue));
        }
        auto lineStr = util::join(line, ' ');
        lines.push_back(std::move(lineStr));
        if (i == 0)
        {
            break;
        }
    }

    util::writeLinesToFile("./cora.cos", lines);
}

int main()
{
    // initEdgeFeature();
    calculateCosine();
    return 0;
}