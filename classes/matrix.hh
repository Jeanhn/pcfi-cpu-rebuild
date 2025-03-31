#ifndef __MATRIXHH__
#define __MATRIXHH__
#include <vector>

namespace pcfi
{
    class Matrix
    {
    private:
        std::vector<std::vector<float>> data;

        const std::vector<float> &getLine(int) const;
        std::vector<float> getColomn(int) const;
        int getLineSize() const;
        int getColomnSize() const;
        float multiple(const std::vector<float> &a, const std::vector<float> &b) const;

    public:
        Matrix(std::vector<std::vector<float>>);
        Matrix multiple(const Matrix &matrix) const;
        void Reserve();
        std::vector<std::vector<float>> &exportData();
        void debug();
    };

    static std::vector<std::vector<float>> zeroMatrix(int line, int colomn);
    static std::vector<std::vector<float>> identifyMatrix(int line);

} // namespace pcfi

#endif