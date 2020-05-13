#include "RecDesc.h"
// #include <fstream>
#include <cstdio>
#include <cstring>
#include <stdexcept>
	


RecDesc::RecDesc(const char* filename)
{
    FILE* file = std::fopen(filename, "r");
    if (!file)
        throw std::invalid_argument(filename);
    std::fseek(file, 0, SEEK_END);
    std::size_t bufSize = std::ftell(file);
    std::fseek(file, 0, 0);
    m_string = std::make_unique<char[]>(bufSize+1);
    std::fread(m_string.get(), sizeof(m_string.get()[0]), bufSize, file);
    std::fclose(file);
 
}

inline static bool isVar(char x)
{
    return ( x == 'x' || x == 'y' || x == 'z');
}

std::shared_ptr<RecDesc::Token>  RecDesc::Tokenize()
{

    char delim[] = " \t\n\r";
    char* token = std::strtok(m_string.get(), delim);
    
    std::shared_ptr<Token> first = nullptr;
    std::shared_ptr<Token>  last = nullptr;

    while (token != nullptr) {
        
        NumTree::t_t tp = NAN;
        char* p = nullptr;
        std::strtod(token, &p);
        auto len = std::strlen(token);

        if (len == 1 &&  isVar(*token))
            tp = VARIABLE;

        #define CMD(d_op, d_sym) \
        else if (len == 1 && *token == *( #d_sym ) )\
            tp = d_op;

        OPERS

        #undef CMD

        else if (len == 1 && *token == '(')
            tp = LBRACKET;
        else if (len == 1 && *token == ')')
            tp = RBRACKET;
        else if (*p == 0)
            tp = NUMBER;
        else if (!std::strcmp("sin", token ))
            tp = SIN;
        else if (!std::strcmp("cos", token ))
            tp = COS;
        else
            throw std::logic_error(token);

        if (token == m_string.get())
        {
            first = std::make_shared<Token>(tp, token);
            last = first;
        }
        else
        {
            last->m_next = std::make_shared<Token>(tp, token);
            last = last->m_next;
        }

        token = std::strtok(nullptr, delim);
    }
    return first;

}

std::shared_ptr<NumTree::Node> RecDesc::GetN(std::shared_ptr<Token>& tk)
{
    if (tk->m_type == VARIABLE || tk->m_type == NUMBER )
    {
        auto ret = NumTree(tk->m_type, tk->m_tok);
        tk = tk->m_next ? tk->m_next : tk;
        return ret.GetHead();
    }
    else if (tk->m_type == SIN || tk->m_type == COS)
    {
        auto ret = NumTree(tk->m_type, tk->m_tok).GetHead();
        tk = tk->m_next ? tk->m_next : tk;
        ret->AddRight(GetMD(tk));
        return ret;
    }
    else
        throw std::logic_error(tk->m_tok);
    
}

std::shared_ptr<NumTree::Node> RecDesc::GetBr(std::shared_ptr<Token>& tk)
{
    std::shared_ptr<NumTree::Node> ret = nullptr;

    if (tk->m_type == LBRACKET)
    {
        tk = tk->m_next ? tk->m_next : tk;

        ret = GetPM(tk);
        if (tk->m_type != RBRACKET)
            throw std::logic_error(tk->m_tok);
        tk = tk->m_next ? tk->m_next : tk;
    }
    else
    {
        ret = GetN(tk);
    }
    return ret;
}

std::shared_ptr<NumTree::Node> RecDesc::GetPM(std::shared_ptr<Token>& tk)
{
    auto ret = GetMD(tk);
    while (tk->m_type == PLUS || tk->m_type == MINUS)
    {
        auto tmp = NumTree(tk->m_type, tk->m_tok).GetHead();
        tmp->AddLeft(ret);
        ret = tmp;
        tk = tk->m_next ? tk->m_next : tk;

        ret->AddRight(GetMD(tk));
    }
    return ret;
}

std::shared_ptr<NumTree::Node> RecDesc::GetMD(std::shared_ptr<Token>& tk)
{
    auto ret = GetPow(tk);

    while (tk->m_type == MUL || tk->m_type == DIV)
    {
        auto tmp = NumTree(tk->m_type, tk->m_tok).GetHead();
        tmp->AddLeft(ret);
        ret = tmp;
        tk = tk->m_next ? tk->m_next : tk;

        ret->AddRight(GetPow(tk));
    }
    return ret;
}

std::shared_ptr<NumTree::Node> RecDesc::GetPow(std::shared_ptr<Token>& tk)
{
    auto ret = GetBr(tk);
    while (tk->m_type == POW)
    {
        auto tmp = NumTree(tk->m_type, tk->m_tok).GetHead();
        tmp->AddLeft(ret);
        ret = tmp;
        tk = tk->m_next ? tk->m_next : tk;
        ret->AddRight(GetBr(tk));
    }
    return ret;
}


NumTree RecDesc::Get()
{
    auto tkns = Tokenize();
    NumTree t(nullptr);
    t.SetHead(GetPM(tkns));
    return t;
}