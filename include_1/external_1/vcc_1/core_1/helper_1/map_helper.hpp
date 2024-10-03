#pragma once

#include<map>
#include<algorithm>

namespace vcc
{
    template<typename A, typename B>
    std::pair<B,A> FlipPair(const std::pair<A,B> &pair)
    {
        return std::pair<B,A>(pair.second, pair.first);
    }

    template<typename A, typename B>
    std::map<B,A> FlipMap(const std::map<A,B> &map)
    {
        std::map<B,A> result;
        transform(map.begin(), map.end(), inserter(result, result.begin()), FlipPair<A,B>);
        return result;
    }
}