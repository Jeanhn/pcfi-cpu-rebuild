#ifndef __NODEHH__
#define __NODEHH__
#include <vector>
#include <string>
#include <optional>

namespace pcfi
{
    class Node
    {
    private:
        /* data */
        std::string index;
        std::vector<std::optional<float>> features;
        std::string kind;

    public:
        Node(std::string i, std::vector<std::optional<float>> f, std::string k);
        std::optional<float> getFeature(int featureIndex) const;
        std::vector<std::optional<float>> &exportFeatures();
        std::string getIndex() const;
        std::string getKind() const;
        void resetFeature(int);
    };

} // namespace pcfi

#endif