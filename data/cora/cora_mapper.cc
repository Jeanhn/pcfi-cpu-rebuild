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
    }

    util::writeLinesToFile("./cora.cos", lines);
}

void calculateAVG()
{
    auto featureLines = util::readFileLines("./cora.content");
    std::unordered_map<std::string, std::vector<int>> allKinds;
    int q = 0;
    for (auto cite : featureLines)
    {
        auto art = util::split(cite);
        if (allKinds.count(art.back()) == 0)
        {
            allKinds.insert({art.back(), std::vector<int>{q}});
        }
        else
        {
            allKinds[art.back()].push_back(q);
        }
        q++;
    }

    auto cosLines = util::readFileLines("./cora.cos");
    std::vector<std::vector<float>> cosMap;
    cosMap.resize(cosLines.size());
    q = 0;
    for (auto &cosLine : cosLines)
    {
        auto cosLineSplit = util::split(cosLine);
        for (auto s : cosLineSplit)
        {

            cosMap[q].push_back(std::stof(s));
        }
        q++;
    }

    std::unordered_map<std::string, float> allKindsSum;
    for (auto &p : allKinds)
    {
        float s = 0;
        int count = 0;
        for (int i = 0; i < p.second.size(); i++)
        {
            for (int j = i + 1; j < p.second.size(); j++)
            {
                try
                {
                    s += cosMap.at(p.second[i]).at(p.second[j] - p.second[i]);
                    count++;
                }
                catch (std::exception e)
                {
                    util::debugOutput("cross", p.second[i], p.second[j], '\n');
                    std::abort();
                }
            }
        }
        allKindsSum[p.first] = s / count;
    }

    float bigAll = 0;
    int count = 0;
    for (int i = 0; i < cosMap.size(); i++)
    {
        for (int j = 0; j < cosMap[i].size(); j++)
        {
            bigAll += cosMap[i][j];
            count++;
        }
    }

    float bvg = bigAll / count;
    std::cout << "total  " << bigAll / count << '\n';

    for (auto p : allKindsSum)
    {
        std::cout << p.first << "   " << p.second << "  " << (bvg > p.second) << '\n';
    }
}

void genEdge()
{
    auto cosLines = util::readFileLines("./cora.cos");
    std::vector<std::vector<float>> cosMap;
    cosMap.resize(cosLines.size());
    int q = 0;
    for (auto &cosLine : cosLines)
    {
        auto cosLineSplit = util::split(cosLine);
        for (auto s : cosLineSplit)
        {

            cosMap[q].push_back(std::stof(s));
        }
        q++;
    }

    auto featureLines = util::readFileLines("./cora.content");
    std::vector<std::string> index;
    for (auto cite : featureLines)
    {
        auto art = util::split(cite);
        index.push_back(art.front());
    }

    std::vector<std::string> lines;
    for (int i = 0; i < cosMap.size(); i++)
    {
        for (int j = 0; j < cosMap[i].size(); j++)
        {
            if (cosMap[i][j] > 0.06)
            {
                auto n1 = index.at(i);
                auto n2 = index.at(j + i);
                lines.push_back(n1 + " " + n2);
            }
        }
    }

    // util::debugOutput(index);

    util::writeLinesToFile("./cora.f2e", lines);
}

int main()
{
    genEdge();
    return 0;
}