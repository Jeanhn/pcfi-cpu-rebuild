#ifndef __UTILHH__
#define __UTILHH__
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <optional>

namespace util
{
    inline void debugOutput(std::vector<std::optional<float>> arg)
    {
        for (auto &t : arg)
        {
            if (!t.has_value())
            {
                std::cout << "x" << ' ';
            }
            else
            {
                std::cout << t.value() << ' ';
            }
        }
        std::cout << std::endl;
    }

    template <typename T>
    inline void debugOutput(std::vector<T> arg)
    {
        for (auto &t : arg)
        {
            std::cout << t << ' ';
        }
        std::cout << std::endl;
    }

    template <typename T>
    inline void debugOutput(T arg)
    {
        std::cout << arg << std::endl;
    }

    template <typename T, typename... Args>
    inline void debugOutput(T arg, Args... args)
    {
        std::cout << arg << std::endl;
        debugOutput(args...);
    }

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
            throw std::runtime_error("文件无法打开:" + filename);
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
            if (it != end)
            {
                result.push_back({it, end});
            }
            it = end;
        }
        return result;
    }

    inline std::vector<std::string> split(const std::string &line)
    {
        auto is_legal_char = [](char c)
        {
            if (c >= 'a' && c <= 'z')
            {
                return true;
            }
            if (c >= 'A' && c <= 'Z')
            {
                return true;
            }
            if (c >= '0' && c <= '9')
            {
                return true;
            }
            if (c == '-' ||
                c == '_')
            {
                return true;
            }
            return false;
        };

        std::vector<std::string> result;
        auto it = line.begin();
        while (it != line.end())
        {
            while (it != line.end() && !is_legal_char(*it))
            {
                it++;
            }
            auto end = it;
            while (end != line.end() && is_legal_char(*end))
            {
                end++;
            }
            if (it != end)
            {
                result.push_back({it, end});
            }
            it = end;
        }
        return result;
    }

    inline std::string join(const std::vector<std::string> &words, char c)
    {
        std::string buf;
        for (int i = 0; i < words.size() - 1; i++)
        {
            buf.append(words[i]);
            buf.push_back(c);
        }
        buf.append(words.back());
        return buf;
    }

    inline void writeLinesToFile(const std::string &filename, const std::vector<std::string> &lines)
    {
        std::ofstream file;
        // 以追加模式打开文件
        file.open(filename, std::ios::out);

        if (!file.is_open())
        {
            throw std::runtime_error("open failed:" + filename);
        }

        for (auto &l : lines)
        {
            file << l << std::endl;
        }
    }

    inline float safeDevide(float a, float b)
    {
        if (b * 100 < a)
        {
            return 100;
        }
        return a / b;
    }
}
#endif