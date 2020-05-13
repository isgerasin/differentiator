#pragma once
#include "Tree.hpp"



class Differenciator
{
public:
    inline static NumTree Diff(const NumTree& t)
    {
        auto ret = DoDiff(t);
        Simplify(ret.GetHead());
        return ret;
    };

private:
    static NumTree DoDiff(const NumTree& t);
    static void Simplify(std::shared_ptr<NumTree::Node> n);
};






