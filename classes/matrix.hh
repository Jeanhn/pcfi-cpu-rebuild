#ifndef __MATRIXHH__
#define __MATRIXHH__
#include <vector>

namespace pcfi
{
    class Matrix
    {
    private:
        std::vector<std::vector<float>> data;

    public:
        Matrix(/* args */);
        ~Matrix();
        Matrix(std::vector<std::vector<float>>);
        Matrix multiple(const Matrix &matrix) const;
        void Reserve();
        std::vector<std::vector<float>> &exportData();
    };

} // namespace pcfi

#endif