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

    std::vector<std::string> readFileLines(const std::string &filename)
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

    float confidence(float base, float time)
    {
        return pow(base, time);
    }
}
#endif