#include <node.hh>

namespace pcfi
{
    Node::Node(std::string &i, std::vector<std::optional<float>> &f) : index(std::move(i)), features(std::move(f))
    {
    }

    std::optional<float> Node::getFeature(int featureIndex) const
    {
        return features.at(featureIndex);
    }

    std::string Node::getIndex() const
    {
        return index;
    }

    std::vector<std::optional<float>> &Node::exportFeatures()
    {
        return features;
    }

} // namespace pcfi
