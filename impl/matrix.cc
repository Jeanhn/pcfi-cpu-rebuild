#include <matrix.hh>

namespace pcfi
{
    Matrix Matrix::multiple(const Matrix &matrix) const;
    void Matrix::Reserve();
    std::vector<std::vector<float>> &Matrix::exportData();
} // namespace pcfi
