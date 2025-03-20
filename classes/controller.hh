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
        std::unordered_map<std::string, int> nodeIndex;
        std::unordered_set<std::pair<std::string, std::string>> edges;
        int featureSize;
        float alpha;

        std::pair<std::vector<Node *>, std::vector<Node *>> distinguish(int featureIndex);
        std::unordered_map<std::string, int> calculateShortestDistance(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes);
        std::unordered_map<std::string, float> calculatePseudoConfidence(const std::unordered_map<std::string, int> &distance);
        Matrix calculateRelativePC(const std::pair<std::vector<Node *>, std::vector<Node *>> &sourceAndMissingNodes, std::unordered_map<std::string, float> &confidenceMap);
        Matrix getFeatureColomn(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes, int featureIndex);
        Matrix sort(const std::vector<std::vector<float>> &);
        std::vector<float> diffusion(const Matrix &, const Matrix &);

    public:
        Controller(/* args */);
        ~Controller();

        void iterate(int featureIndex);
    };

} // namespace pcfi

#endif