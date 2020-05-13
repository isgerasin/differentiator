#include "Diff.h"
#include <cstring>

NumTree Differenciator::DoDiff(const NumTree& t)
{
    static char two[] = "2";
    static char one[] = "1";
    static char zero[] = "0";
    static char mone[] = "-1";
    static char cos[] = "cos";
    static char sin[] = "sin";


    const auto h = t.GetHead();
    const auto lhs = h->GetLeft();
    const auto rhs = h->GetRight();
    NumTree tmp(nullptr);
    switch (h->GetType())
    {
    case node_t::PLUS:
        return Diff(lhs) + Diff(rhs);
        break;
    
    case node_t::MINUS:
        return Diff(lhs) - Diff(rhs);
        break;
    
    case node_t::MUL:
        return (Diff(lhs)    * NumTree(rhs) )+
               (NumTree(lhs) * Diff(rhs));
        break;
    
    case node_t::DIV:
        return ((Diff(lhs)* NumTree(rhs)) - (NumTree(lhs) * Diff(rhs)) ) /
                         (NumTree(rhs)^NumTree(NUMBER, two));
        break;
    

    case node_t::SIN:
        tmp = NumTree(COS, cos);
        tmp.GetHead()->m_right = rhs;
        return (tmp *  Diff(rhs));
        break;
    
    case node_t::COS:
        return NumTree(NUMBER, mone) * NumTree(COS, sin)  *  Diff(rhs);
        break;
    

    case node_t::NUMBER:
        return NumTree(NUMBER, zero);
        break;

    case node_t::VARIABLE:
        return NumTree(NUMBER, one);
        break;

    default:
        throw std::logic_error("Unknown operation");
        break;
    }
    return NumTree(NUMBER, zero);
}

void Differenciator::Simplify(std::shared_ptr<NumTree::Node> n)
{
    static char zero[] = "0";
    if (n == nullptr || n->GetLeft() == nullptr || n->GetRight() == nullptr)
        return;
    

    Simplify(n->GetLeft());
    Simplify(n->GetRight());

    auto tp = n->GetType();
    auto rhs = n->GetRight();
    auto lhs = n->GetLeft();
    auto ld = lhs->GetData();
    auto rd = rhs->GetData();

    if (tp == PLUS && !std::strcmp("0", ld))
    {
        n->m_type = rhs->GetType();
        n->m_data = rd;
        n->m_left = rhs->GetLeft();
        n->m_right = rhs->GetRight();
    }
    else if((tp == PLUS || tp == MINUS) && !std::strcmp("0", rd))
    {
        n->m_type = lhs->GetType();
        n->m_data = ld;
        n->m_left = lhs->GetLeft();
        n->m_right = lhs->GetRight();
    }
    else if(tp == MUL && !std::strcmp("1", rd))
    {
        n->m_type = lhs->GetType();
        n->m_data = ld;
        n->m_left = lhs->GetLeft();
        n->m_right = lhs->GetRight();
    }
    else if (tp == MUL && !std::strcmp("1", ld))
    {
        n->m_type = rhs->GetType();
        n->m_data = rd;
        n->m_left = rhs->GetLeft();
        n->m_right = rhs->GetRight();
    }
    else if (tp == MUL && (!std::strcmp("0", ld) || !std::strcmp("0", rd)))
    {
        n->m_type = NUMBER;
        n->m_data = zero;
        n->m_left = nullptr;
        n->m_right = nullptr;
    }


}