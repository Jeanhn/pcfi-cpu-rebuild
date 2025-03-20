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
        ~Node();
        std::optional<float> getFeature(int featureIndex);
        std::string getIndex();
    };

} // namespace pcfi

#endif