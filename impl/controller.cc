#include <controller.hh>
#include <util.hh>
#include <algorithm>
#include <numeric>
#include <queue>

namespace pcfi
{
    Controller::Controller(std::vector<Node *> nodeArray,
                           std::vector<std::pair<std::string, std::string>> edgeArray,
                           float a,
                           int r)
        : nodes(std::move(nodeArray)), edges(std::move(edgeArray)), alpha(a), round(r)
    {
    }

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

    void Controller::sort(std::vector<std::vector<float>> &relativeMatrix, int sourceSize)
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

    void Controller::diffusion(const Matrix &diffusionMatrix, Matrix &featureColomn)
    {
        featureColomn = std::move(diffusionMatrix.multiple(featureColomn));
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

    std::vector<float> Controller::iterate(int featureIndex)
    {
        auto sourceAndMissingNodes = distinguish(featureIndex);
        if (sourceAndMissingNodes.second.empty())
        {
            std::vector<float> result;
            result.reserve(nodes.size());
            for (int i = 0; i < nodes.size(); i++)
            {
                result.push_back(nodes[i]->getFeature(featureIndex).value());
            }
            return result;
        }

        auto distanceMap = calculateShortestDistance(sourceAndMissingNodes);
        auto pseudoConfidenceMap = calculatePseudoConfidence(distanceMap);

        Matrix relativePC = calculateRelativePC(sourceAndMissingNodes, pseudoConfidenceMap);

        sort(relativePC.exportData(), sourceAndMissingNodes.first.size());

        Matrix featureColomn = getFeatureColomn(sourceAndMissingNodes, featureIndex);

        for (int i = 0; i < round; i++)
        {
            diffusion(relativePC, featureColomn);
        }

        auto srcIter = featureColomn.exportData().begin();
        auto missIter = std::next(srcIter, sourceAndMissingNodes.first.size());
        std::vector<float> result;
        result.reserve(featureColomn.exportData().size());

        for (Node *n : nodes)
        {
            if (auto feature = n->getFeature(featureIndex); feature.has_value())
            {
                result.push_back((*srcIter)[0]);
                srcIter++;
            }
            else
            {
                result.push_back((*missIter)[0]);
                missIter++;
            }
        }
        return result;
    }

    int Controller::featureSize() const
    {
        return nodes.at(0)->exportFeatures().size();
    }

    void Controller::fixNodes(std::vector<std::vector<float>> features)
    {
        if (features.size() != nodes.size())
        {
            throw std::runtime_error("fix node matrix mismatch");
        }
        for (int i = 0; i < features.size(); i++)
        {
            if (nodes[i]->exportFeatures().size() != features[i].size())
            {
                throw std::runtime_error("fix node line mismatch");
            }
            for (int j = 0; j < nodes[i]->exportFeatures().size(); j++)
            {
                nodes[i]->exportFeatures().at(j) = std::optional<float>(features[i][j]);
            }
        }
    }

    void Controller::saveTo(const std::string &path, bool generatingRandomMissing)
    {
        std::vector<std::string> lines;
        for (auto n : nodes)
        {
            std::string index = n->getIndex(),
                        kind = n->getKind();
            std::vector<std::string> line;
            line.reserve(n->exportFeatures().size() + 3);
            line.push_back(index);
            for (std::optional<float> v : n->exportFeatures())
            {
                if (generatingRandomMissing && !v.has_value())
                {
                    line.push_back("x");
                    continue;
                }
                line.push_back(std::to_string(v.value()));
            }
            line.push_back(kind);

            std::string joinedLine = util::join(line, ' ');
            lines.push_back(std::move(joinedLine));
        }
        util::writeLinesToFile(path, lines);
    }

    void Controller::generateRandomMissingFeatures(int num)
    {
        if (num >= nodes.size())
        {
            throw std::runtime_error("too large missing");
        }

        for (int i = 0; i < featureSize(); i++)
        {
            int k = num;
            for (int j = i; j < nodes.size() + i; j++)
            {
                int t = j % nodes.size();
                nodes.at(t)->resetFeature(i);
                k--;
                if (k == 0)
                {
                    break;
                }
            }
            std::cout << std::endl;
        }
    }

}
// namespace pcfi
