#pragma once

#include <vector>

template <typename T>
inline void RemoveVectorIfContainElement(std::vector<T> &sourceVector, const T &obj)
{
    sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(), [&](const T &element) {
        return element == obj;
    }), sourceVector.end());
}

template <typename T>
inline void RemoveVectorIfContainElements(std::vector<T> &sourceVector, const std::vector<T> &obj)
{
    sourceVector.erase(std::remove_if(sourceVector.begin(), sourceVector.end(), [&](const T &element) {
        return std::find(obj.begin(), obj.end(), element) != obj.end();
    }), sourceVector.end());
}