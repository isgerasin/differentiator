#include <cassert>
#include <cstring>
#include "Tree.hpp"
#include "RecDesc.h"
#include "Diff.h"

int Test()
{
    char  two[] = "2";
    char three[] = "3";

    auto n1 = std::make_shared<NumTree::Node>(node_t::NUMBER, two);
    auto n2 = std::make_shared<NumTree::Node>(node_t::NUMBER, three);
    try
    {
        n1->AddRight(n1);
        std::cerr << "No exception\n";
        return -1;
    }
    catch (std::exception& e)
    {
        ;
    }
    n1->AddLeft(n2);
    assert(n1->GetLeft() == n2);
    assert(n1->GetRight() == nullptr);
    assert(n2->GetParent() == n1.get());


    NumTree t1(NUMBER, two);
    NumTree t2(NUMBER, three);

    NumTree t3(t1+t2);
    auto h = t3.GetHead();
    assert(!std::strcmp(h->GetData(), "+"));
    assert(h->GetType() == PLUS);
    assert(h->GetParent() == nullptr);
    assert(h->GetLeft()->GetData() == two);
    assert(h->GetRight()->GetData() == three);
    assert(h->GetLeft()->GetParent() == h.get());
    assert(h->GetRight()->GetParent() == h.get());

    t3 = t1 * t2;
    h = t3.GetHead();
    assert(!std::strcmp(h->GetData(), "*"));
    assert(h->GetType() == MUL);
    assert(h->GetParent() == nullptr);
    assert(h->GetLeft()->GetData() == two);
    assert(h->GetRight()->GetData() == three);
    assert(h->GetLeft()->GetParent() == h.get());
    assert(h->GetRight()->GetParent() == h.get());
    assert(h->GetRight()->GetRight() == nullptr);

    auto filename = "../testVar.txt";
    RecDesc rd(filename);
    auto tree = rd.Get();

    assert(tree.GetHead()->GetType() == MINUS);
    assert(tree.GetHead()->GetLeft()->GetType() == PLUS);
    assert(tree.GetHead()->GetRight()->GetType() == POW);

    auto filename2 = "../testVar.tex";
    tree.Dump(filename2);
    system("pdftex ../testVar.tex \\\\end");

    return 0;
}

int main(int argc, char* argv[])
{
    // Test();
    auto filename = "../Var.txt";
    if (argc > 2)
    {
        filename = argv[1];
    }
    RecDesc rd(filename);
    auto tree = rd.Get();

    auto filename2 = "../Orig.tex";
    tree.Dump(filename2);

    tree = Differenciator::Diff(tree);


    auto filename3 = "../Diff.tex";
    tree.Dump(filename3);
    system("pdftex ../Diff.tex \\\\end");
    system("pdftex ../Orig.tex \\\\end");
    return 0;
}