#ifndef __SQUAREHH__
#define __SQUAREHH__
#include <memory>

namespace pcfi
{
    class Square
    {
    private:
        std::unique_ptr<float> data;
        int rowSize, colSize;

    public:
        Square(int row, int col);
        float &at(int r, int c);
        const float &atConst(int r, int c) const;
        void multiple(const Square &, Square &);
        std::pair<int, int> shape() const;
    };

} // namespace pcfi

#endif