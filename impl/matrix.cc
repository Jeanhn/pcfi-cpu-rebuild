#include <matrix.hh>
#include <stdexcept>

namespace pcfi
{
    int Matrix::getLineSize() const
    {
        return data.size();
    }

    int Matrix::getColomnSize() const
    {
        return data.at(0).size();
    }

    std::vector<float> Matrix::getLine(int index) const
    {
        return data[index];
    }

    std::vector<float> Matrix::getColomn(int index) const
    {
        std::vector<float> colomn;
        for (int i = 0; i < data.size(); i++)
        {
            colomn.push_back(data[i][index]);
        }
        return colomn;
    }

    float Matrix::multiple(const std::vector<float> &a, const std::vector<float> &b) const
    {
        if (a.size() != b.size())
        {
            throw std::runtime_error("array size mismatch");
        }

        float result = 0;
        for (int i = 0; i < a.size(); i++)
        {
            result += a[i] * b[i];
        }
        return result;
    }

    Matrix Matrix::multiple(const Matrix &matrix) const
    {
        if (this->getColomnSize() != matrix.getLineSize())
        {
            throw std::runtime_error("matrix shape mismatch");
        }

        std::vector<std::vector<float>> result;
        result.resize(this->getLineSize());
        for (int i = 0; i < this->getLineSize(); i++)
        {
            result[i].resize(matrix.getColomnSize());
            auto line = this->getLine(i);
            for (int j = 0; j < matrix.getColomnSize(); j++)
            {
                auto colomn = matrix.getColomn(j);
                float t = multiple(line, colomn);
                result[i][j] = t;
            }
        }

        return std::move(result);
    }

    void Matrix::Reserve()
    {
        std::vector<std::vector<float>> base;
        for (int i = 0; i < this->getColomnSize(); i++)
        {
            auto colomn = this->getColomn(i);
            base.push_back(std::move(colomn));
        }

        this->data = std::move(base);
    }

    std::vector<std::vector<float>> &Matrix::exportData()
    {
        return data;
    }

    static Matrix zeroMatrix(int line, int colomn)
    {
        std::vector<std::vector<float>> base;
        base.resize(line);
        for (int i = 0; i < line; i++)
        {
            base[i].resize(colomn);
        }
        return std::move(base);
    }

    static Matrix identifyMatrix(int line)
    {
        Matrix m = zeroMatrix(line, line);
        auto &d = m.exportData();
        for (int i = 0; i < line; i++)
        {
            d[i][i] = 1;
        }
        return m;
    }

} // namespace pcfi
