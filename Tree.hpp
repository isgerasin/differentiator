#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <functional>
#include <memory>
#include <fstream>
#include <cstdlib>

#define OPERS \
        CMD(PLUS, +) \
        CMD(MINUS, -) \
        CMD(MUL, *) \
        CMD(DIV, /) \
        CMD(POW, ^)



enum node_t
{
    #define CMD(d_op, d_sym) d_op,
    OPERS
    #undef CMD

    NUMBER,
    VARIABLE,
    LBRACKET,
    RBRACKET,
    SIN,
    COS,
    NAN
};

class Differenciator;

template<typename data_t, typename type_t>
class Tree
{
public:
    using d_t = data_t;
    using t_t = type_t;

    class Node{
    public:
        Node(type_t t, data_t d);
        ~Node();

        void AddLeft(std::shared_ptr<Node> l);
        void AddRight(std::shared_ptr<Node> r);

        std::shared_ptr<Node> DeepCopy() const;

        inline Node* GetParent() const { return m_parent; };
        inline std::shared_ptr<Node> GetLeft() const   { return m_left;   };
        inline std::shared_ptr<Node> GetRight() const  { return m_right;  };
        inline data_t GetData() const  { return m_data;   };
        inline type_t GetType() const  { return m_type;   };

        friend Differenciator;

    private:
        type_t m_type;
        data_t m_data;

        Node* m_parent;
        std::shared_ptr<Node> m_left;
        std::shared_ptr<Node> m_right;
    };
    
    Tree(type_t t, data_t d);
    Tree(std::shared_ptr<Node> n);
    Tree(const Tree& t);
    Tree(Tree&& t);


    inline void swap(Tree& t) { std::swap(m_head, t.m_head); };
    const Tree operator=(const Tree&);

    void Dump(const char* filename);

    #define CMD(d_op, d_sym) \
    friend const Tree<char*, node_t> operator d_sym (const Tree<char*, node_t>& lhs, const Tree<char*, node_t>& rhs);

    OPERS

    #undef CMD

    inline std::shared_ptr<Node> GetHead() { return m_head; };
    inline std::shared_ptr<const Node> GetHead() const { return m_head; };
    inline void SetHead(std::shared_ptr<Node> n){ m_head = n; };
    
private:
    
    static void DumpNode(std::ofstream& out, std::shared_ptr<Node> n);

    std::shared_ptr<Node> m_head;
};

using NumTree = Tree<char*, node_t>;


#define CMD(d_op, d_sym)\
inline const Tree<char*, node_t> operator d_sym (const Tree<char*, node_t>& lhs, const Tree<char*, node_t>& rhs) \
{                                               \
    static char s[] = #d_sym;                   \
    Tree<char*, node_t> t(d_op, s);            \
    t.m_head->AddLeft(lhs.m_head->DeepCopy());  \
    t.m_head->AddRight(rhs.m_head->DeepCopy()); \
    return t;                                   \
}

OPERS

#undef CMD

template<typename data_t, typename type_t>
Tree<data_t, type_t>::Node::Node(type_t t, data_t d):
    m_type(t),
    m_data(d),
    m_parent(nullptr),
    m_left(nullptr),
    m_right(nullptr)
{}

template<typename data_t, typename type_t>
Tree<data_t, type_t>::Node::~Node()
{
    m_parent = nullptr;
    m_left = nullptr;
    m_right = nullptr;
}


template<typename data_t, typename type_t>
void Tree<data_t, type_t>::Node::AddLeft(std::shared_ptr<Node> l)
{
    if (l.get() == this)
        throw std::invalid_argument("Trying to add self to left");
    
    l->m_parent = this;
    m_left = l;
}

template<typename data_t, typename type_t>
void Tree<data_t, type_t>::Node::AddRight(std::shared_ptr<Node> r)
{
    if (r.get() == this)
        throw std::invalid_argument("Trying to add self to right");
    
    r->m_parent = this;
    m_right = r;
}



template<typename data_t, typename type_t>
std::shared_ptr<typename  Tree<data_t, type_t>::Node> Tree<data_t, type_t>::Node::DeepCopy() const
{
    // auto ret = new Node(m_type, m_data);
    auto ret = std::make_shared<Node>(m_type, m_data);
    if (m_left)
    {
    ret->AddLeft(m_left->DeepCopy());
    }
    if (m_right)
    {
        ret->AddRight(m_right->DeepCopy());
    }
    return ret;
}

template<typename data_t, typename type_t>
Tree<data_t, type_t>::Tree(type_t t, data_t d)
{
    // m_head = new Node(t, d);
    m_head = std::make_shared<Node>(t, d);
}

template<typename data_t, typename type_t>
Tree<data_t, type_t>::Tree(const Tree& t)
{
    m_head = t.m_head->DeepCopy();
}

template<typename data_t, typename type_t>
Tree<data_t, type_t>::Tree(Tree&& t)
{
    m_head = t.m_head;
    t.m_head = nullptr;
}

template<typename data_t, typename type_t>
Tree<data_t, type_t>::Tree(std::shared_ptr<Node> n)
{
    if (n != nullptr)
        m_head = n->DeepCopy();
    else
        m_head = nullptr;
}


template<typename data_t, typename type_t>
void Tree<data_t, type_t>::Dump(const char* filename)
{
    std::ofstream out(filename);
    out << "$ ";

    DumpNode(out, m_head);

    out << "$ ";

}


template<typename data_t, typename type_t>
void Tree<data_t, type_t>::DumpNode(std::ofstream& out, std::shared_ptr<Node> n)
{
    if (n == nullptr)
        return;
    
    out << "{";
    if (n->GetType() != NUMBER && n->GetType() != POW && n->GetType() != MUL && n->GetType() != VARIABLE)
        out << "( ";
        
    DumpNode(out, n->GetLeft());

    out << n->GetData() ;

    DumpNode(out, n->GetRight());

    if (n->GetType() != NUMBER && n->GetType() != POW && n->GetType() != MUL && n->GetType() != VARIABLE)
        out << " )";
    out << " }";
}

template<typename data_t, typename type_t>
const Tree<data_t, type_t> Tree<data_t, type_t>::operator= (const Tree<data_t, type_t>& t)
{
    Tree tmp(t);
    swap(tmp);
    return *this;
}
