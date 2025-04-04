#include <square.hh>
#include <stdexcept>
#include <util.hh>

namespace pcfi
{

    Square::Square(int row, int col) : rowSize(row), colSize(col)
    {
        float *d = new float[row * col];
        data.reset(d);
    }

    Square::Square(const std::vector<std::vector<float>> &matrix) : rowSize(matrix.size()), colSize(matrix[0].size())
    {
        float *d = new float[rowSize * colSize];
        for (int i = 0; i < rowSize; i++)
        {
            for (int j = 0; j < colSize; j++)
            {
                d[i * colSize + j] = matrix[i][j];
            }
        }

        data.reset(d);
    }

    void Square::debug() const
    {
        float *d = data.get();
        for (int i = 0; i < rowSize; i++)
        {
            for (int j = 0; j < colSize; j++)
            {
                std::cout << d[i * colSize + j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    void Square::set(int r, int c, float v)
    {
        float *d = data.get();
        d[r * colSize + c] = v;
    }

    float Square::at(int r, int c) const
    {
        if (r >= rowSize || r < 0 ||
            c >= colSize || c < 0)
        {
            throw std::runtime_error("read beyond size");
        }
        return data.get()[r * colSize + c];
    }

    std::pair<int, int> Square::shape() const
    {
        return {rowSize, colSize};
    }

    void Square::multiple(const Square &matrix, Square &destination) const
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
                    sum += this->at(i, k) * matrix.at(k, j);
                }
                destination.set(i, j, sum);
            }
        }
    }

} // namespace pcfi
