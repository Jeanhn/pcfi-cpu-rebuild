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

    public:
        Node(std::string &i, std::vector<std::optional<float>> &f);
        std::optional<float> getFeature(int featureIndex) const;
        std::vector<std::optional<float>> &exportFeatures();
        std::string getIndex() const;
    };

} // namespace pcfi

#endif