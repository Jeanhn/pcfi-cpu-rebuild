#include <controller.hh>
#include <util.hh>
#include <algorithm>
#include <numeric>
#include <queue>

namespace pcfi
{
    Controller::Controller(std::vector<Node *> nodeArray,
                           std::vector<std::pair<std::string, std::string>> edgeArray)
        : nodes(std::move(nodeArray)), edges(std::move(edgeArray)) {}

    std::pair<std::vector<Node *>, std::vector<Node *>> Controller::distinguish(int featureIndex)
    {
        std::vector<Node *> source, missing;
        for (Node *n : nodes)
        {
            if (auto feature = n->getFeature(featureIndex); feature.has_value())
            {
                source.push_back(n);
            }
            else
            {
                missing.push_back(n);
            }
        }
        return {source, missing};
    }

    std::unordered_map<std::string, int> Controller::calculateShortestDistance(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes)
    {
        std::unordered_map<std::string, int> shortestDistances;
        std::queue<std::pair<std::string, int>> bfsQueue;

        // 初始化源节点的距离为 0
        for (Node *sourceNode : sourceAndMissingNodes.first)
        {
            std::string index = sourceNode->getIndex();
            shortestDistances[index] = 0;
            bfsQueue.push({index, 0});
        }

        // BFS 搜索
        while (!bfsQueue.empty())
        {
            auto [currentIndex, currentDistance] = bfsQueue.front();
            bfsQueue.pop();

            // 遍历所有边，找到相邻节点
            for (const auto &edge : edges)
            {
                if (edge.first == currentIndex)
                {
                    std::string neighborIndex = edge.second;
                    if (shortestDistances.find(neighborIndex) == shortestDistances.end())
                    {
                        shortestDistances[neighborIndex] = currentDistance + 1;
                        bfsQueue.push({neighborIndex, currentDistance + 1});
                    }
                }
                else if (edge.second == currentIndex)
                {
                    std::string neighborIndex = edge.first;
                    if (shortestDistances.find(neighborIndex) == shortestDistances.end())
                    {
                        shortestDistances[neighborIndex] = currentDistance + 1;
                        bfsQueue.push({neighborIndex, currentDistance + 1});
                    }
                }
            }
        }

        // 处理未访问到的缺失节点
        for (Node *missingNode : sourceAndMissingNodes.second)
        {
            std::string index = missingNode->getIndex();
            if (shortestDistances.find(index) == shortestDistances.end())
            {
                shortestDistances[index] = INT16_MAX;
            }
        }

        return shortestDistances;
    }

    std::unordered_map<std::string, float> Controller::calculatePseudoConfidence(const std::unordered_map<std::string, int> &distance)
    {
        std::unordered_map<std::string, float> result;

        for (auto p : distance)
        {
            float pseudoConfidence = util::confidence(alpha, (float)p.second);
            result.insert({p.first, pseudoConfidence});
        }

        return result;
    }

    Matrix Controller::calculateRelativePC(const std::pair<std::vector<Node *>, std::vector<Node *>> &sourceAndMissingNodes, std::unordered_map<std::string, float> &confidenceMap)
    {
        std::vector<Node *> sortedNodes;
        std::copy(sourceAndMissingNodes.first.begin(), sourceAndMissingNodes.first.end(), std::back_inserter(sortedNodes));
        std::copy(sourceAndMissingNodes.second.begin(), sourceAndMissingNodes.second.end(), std::back_inserter(sortedNodes));

        std::vector<std::vector<float>> relativeMatrix;
        relativeMatrix.resize(nodes.size());
        for (int i = 0; i < nodes.size(); i++)
        {
            relativeMatrix[i].resize(nodes.size());
            float c1 = confidenceMap[sortedNodes[i]->getIndex()];
            for (int j = 0; j < nodes.size(); j++)
            {
                float c2 = confidenceMap[sortedNodes[j]->getIndex()];
                relativeMatrix[i][j] = c2 / c1;
            }
        }

        for (int i = 0; i < nodes.size(); i++)
        {
            int sum = std::accumulate(relativeMatrix[i].begin(), relativeMatrix[i].end(), 0);
            for (int j = 0; j < nodes.size(); j++)
            {
                relativeMatrix[i][j] /= sum;
            }
        }

        Matrix result(std::move(relativeMatrix));
        return result;
    }

    Matrix Controller::sort(std::vector<std::vector<float>> &relativeMatrix, int sourceSize)
    {
        for (int i = 0; i < relativeMatrix.size(); i++)
        {
            for (int j = 0; j < relativeMatrix[i].size(); j++)
            {
                if (i < sourceSize)
                {
                    if (i == j)
                    {
                        relativeMatrix[i][j] = 1;
                    }
                    else
                    {
                        relativeMatrix[i][j] = 0;
                    }
                }
            }
        }
    }

    std::vector<float> Controller::diffusion(const Matrix &diffusionMatrix, const Matrix &featureColomn)
    {
        return {};
    }

    Matrix Controller::getFeatureColomn(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes, int featureIndex)
    {
        std::vector<std::vector<float>> m;
        for (int i = 0; i < sourceAndMissingNodes.first.size(); i++)
        {
            std::vector<float> line = {sourceAndMissingNodes.first.at(i)->getFeature(featureIndex).value()};
            m.push_back(line);
        }
        for (int i = 0; i < sourceAndMissingNodes.second.size(); i++)
        {
            std::vector<float> line = {0};
            m.push_back(line);
        }
        return std::move(m);
    }

    void Controller::iterate(int featureIndex)
    {
        auto sourceAndMissingNodes = distinguish(featureIndex);
        auto distanceMap = calculateShortestDistance(sourceAndMissingNodes);
        auto pseudoConfidenceMap = calculatePseudoConfidence(distanceMap);

        Matrix relativePC = calculateRelativePC(sourceAndMissingNodes, pseudoConfidenceMap);

        Matrix sortedDiffusionMatirx = sort(relativePC.exportData(), sourceAndMissingNodes.first.size());

        Matrix featureColomn = getFeatureColomn(sourceAndMissingNodes, featureIndex);

        for (int i = 0; i < 10; i++)
        {
            diffusion(sortedDiffusionMatirx, featureColomn);
        }
    }
}
// namespace pcfi
