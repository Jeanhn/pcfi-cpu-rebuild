#include <controller.hh>
#include <util.hh>

namespace pcfi
{
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

        Matrix result(std::move(relativeMatrix));
        return result;
    }

    Matrix Controller::sort(const std::vector<std::vector<float>> &relativeMatrix)
    {
    }

    std::vector<float> Controller::diffusion(const Matrix &diffusionMatrix, const Matrix &featureColomn)
    {
        Matrix result = diffusionMatrix.multiple(featureColomn);
    }

    void Controller::iterate(int featureIndex)
    {
        auto sourceAndMissingNodes = distinguish(featureIndex);
        auto distanceMap = calculateShortestDistance(sourceAndMissingNodes);
        auto pseudoConfidenceMap = calculatePseudoConfidence(distanceMap);

        Matrix relativePC = calculateRelativePC(sourceAndMissingNodes, pseudoConfidenceMap);

        Matrix sortedDiffusionMatirx = sort(relativePC.exportData());

        Matrix featureColomn = getFeatureColomn(sourceAndMissingNodes, featureIndex);

        for (int i = 0; i < 10; i++)
        {
            diffusion(sortedDiffusionMatirx, featureColomn);
        }
    }
}
// namespace pcfi
