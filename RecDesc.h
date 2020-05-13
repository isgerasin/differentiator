#pragma once
#include <memory>
#include "Tree.hpp"

class RecDesc
{
public:

    RecDesc(const char* filename);
    NumTree Get();


private:


    struct Token
    {
        Token(NumTree::t_t t, char* tk):
            m_type(t),
            m_tok(tk),
            m_next(nullptr)
        {}

        NumTree::t_t m_type;
        char* m_tok;
        std::shared_ptr<Token> m_next;
    };

    std::shared_ptr<Token>  Tokenize();

    std::shared_ptr<NumTree::Node> GetN(std::shared_ptr<Token>& tk);
    std::shared_ptr<NumTree::Node> GetBr(std::shared_ptr<Token>& tk);
    std::shared_ptr<NumTree::Node> GetPM(std::shared_ptr<Token>& tk);
    std::shared_ptr<NumTree::Node> GetMD(std::shared_ptr<Token>& tk);
    std::shared_ptr<NumTree::Node> GetPow(std::shared_ptr<Token>& tk);
    // std::shared_ptr<NumTree::Node> GetSin(std::shared_ptr<Token>& tk);


    std::unique_ptr<char[]> m_string;
};