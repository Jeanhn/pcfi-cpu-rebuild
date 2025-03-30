#ifndef __UTILHH__
#define __UTILHH__
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

namespace util
{

    inline std::vector<std::string> readFileLines(const std::string &filename)
    {
        std::vector<std::string> lines;
        std::ifstream file(filename);

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                lines.push_back(line);
            }
            file.close();
        }
        else
        {
            throw std::runtime_error("文件无法打开");
        }

        return lines;
    }

    inline float confidence(float base, float time)
    {
        return pow(base, time);
    }

    inline std::vector<std::string> split(const std::string &line, char c)
    {
        std::vector<std::string> result;
        auto it = line.begin();
        while (it != line.end())
        {
            while (it != line.end() && *it == c)
            {
                it++;
            }
            auto end = it;
            while (end != line.end() && *end != c)
            {
                end++;
            }
            result.push_back({it, end});
            it = end;
        }
        return result;
    }
}
#endif