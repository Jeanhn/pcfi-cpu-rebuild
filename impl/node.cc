#include <node.hh>

namespace pcfi
{
    Node::Node(std::string i, std::vector<std::optional<float>> f, std::string k) : index(std::move(i)), features(std::move(f)), kind(std::move(k))
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

    std::string Node::getKind() const
    {
        return kind;
    }

    std::vector<std::optional<float>> &Node::exportFeatures()
    {
        return features;
    }

    void Node::resetFeature(int i)
    {
        features.at(i).reset();
    }
} // namespace pcfi
