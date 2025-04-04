#ifndef __GRAPHHH__
#define __GRAPHHH__
#include <node.hh>
#include <matrix.hh>
#include <unordered_map>
#include <unordered_set>
#include <square.hh>

namespace pcfi
{
    class Controller
    {
    private:
        /* data */
        std::vector<Node *> nodes;
        std::vector<std::pair<std::string, std::string>> edges;
        float alpha = 0.8;
        int round;

    public:
        std::pair<std::vector<Node *>, std::vector<Node *>> distinguish(int featureIndex);
        std::unordered_map<std::string, int> calculateShortestDistance(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes);
        std::unordered_map<std::string, float> calculatePseudoConfidence(const std::unordered_map<std::string, int> &distance);
        Square calculateRelativePC(const std::pair<std::vector<Node *>, std::vector<Node *>> &sourceAndMissingNodes, std::unordered_map<std::string, float> &confidenceMap);
        Square getFeatureColomn(std::pair<std::vector<Node *>, std::vector<Node *>> sourceAndMissingNodes, int featureIndex);
        void sort(Square &, int sourceSize);
        void diffusion(const Square &, Square &);

    public:
        Controller(std::vector<Node *>, std::vector<std::pair<std::string, std::string>>, float, int r);

        std::vector<float> iterate(int featureIndex);

        int featureSize() const;

        void fixNodes(std::vector<std::vector<float>> features);

        void saveTo(const std::string &, bool = false);

        void generateRandomMissingFeatures(int);
    };

} // namespace pcfi

#endif