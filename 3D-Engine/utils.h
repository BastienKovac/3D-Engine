#ifndef UTILS_H
#define UTILS_H

#include <QColor>
#include <vector>

const double DOUBLE_PI = 3.1415926535897932384626433832795;

inline float normalize(float val, float min, float max)
{
    return (val - min) / (max - min);
}

inline void qColorToRGB(const QColor &C, float &r, float &g, float &b)
{
    r = normalize(C.red(), 1.0f, 255.0f);
    g = normalize(C.green(), 1.0f, 255.0f);
    b = normalize(C.blue(), 1.0f, 255.0f);
}

template<typename  T>
std::vector<T> arange(T start, T stop, T step = 1)
{
    std::vector<T> values;
    for (T value = start ; value < stop ; value += step)
    {
        values.push_back(value);
    }
    return values;
}

#endif // UTILS_H
