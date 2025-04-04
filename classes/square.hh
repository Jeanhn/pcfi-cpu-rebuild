#ifndef __SQUAREHH__
#define __SQUAREHH__
#include <memory>
#include <vector>

namespace pcfi
{
    class Square
    {
    private:
        std::unique_ptr<float> data;
        int rowSize, colSize;

    public:
        Square(int row, int col);
        Square(const std::vector<std::vector<float>> &);
        float at(int r, int c) const;
        void set(int, int, float);
        void multiple(const Square &, Square &) const;
        std::pair<int, int> shape() const;
        void debug() const;
    };

} // namespace pcfi

#endif