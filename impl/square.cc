#include <square.hh>
#include <stdexcept>

namespace pcfi
{

    Square::Square(int row, int col) : rowSize(row), colSize(col)
    {
        float *d = new float(row * col);
        data.reset(d);
    }

    float &Square::at(int r, int c)
    {
        if (r >= rowSize || r < 0 ||
            c >= colSize || c < 0)
        {
            throw std::runtime_error("read beyond size");
        }
        return data.get()[r * c];
    }

    const float &Square::atConst(int r, int c) const
    {
        if (r >= rowSize || r < 0 ||
            c >= colSize || c < 0)
        {
            throw std::runtime_error("read beyond size");
        }
        return data.get()[r * c];
    }

    std::pair<int, int> Square::shape() const
    {
        return {rowSize, colSize};
    }

    void Square::multiple(const Square &matrix, Square &destination)
    {
        if (this->shape().second != matrix.shape().first)
        {
            throw std::runtime_error("square shape error");
        }

        for (int i = 0; i < rowSize; i++)
        {
            for (int j = 0; j < matrix.shape().second; j++)
            {
                float sum = 0;
                for (int k = 0; k < matrix.shape().first; k++)
                {
                    sum += this->atConst(i, k) * matrix.atConst(k, j);
                }
                destination.at(i, j) = sum;
            }
        }
    }

} // namespace pcfi
