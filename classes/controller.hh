#ifndef __GRAPHHH__
#define __GRAPHHH__
#include <node.hh>
#include <matrix.hh>
#include <unordered_map>
#include <unordered_set>

namespace pcfi
{
    class Controller
    {
    private:
        /* data */
        std::vector<Node *> nodes;
        std::unordered_set<std::pair<std::string, std::string>> edges;
        int featureSize;
        float alpha;

        std::pair<std::vector<Node *>, std::vector<Node *>> distinguish(int featureIndex);
        std::unordered_map<std::string, int> calculateShortestDistance(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes);
        std::unordered_map<std::string, float> calculatePseudoConfidence(const std::unordered_map<std::string, int> &distance);
        Matrix calculateRelativePC(const std::pair<std::vector<Node *>, std::vector<Node *>> &sourceAndMissingNodes, std::unordered_map<std::string, float> &confidenceMap);
        Matrix getFeatureColomn(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes, int featureIndex);
        Matrix sort(std::vector<std::vector<float>> &, int sourceSize);
        std::vector<float> diffusion(const Matrix &, const Matrix &);

    public:
        Controller(std::vector<Node *>, std::unordered_set<std::pair<std::string, std::string>>);

        void iterate(int featureIndex);
    };

} // namespace pcfi

#endif