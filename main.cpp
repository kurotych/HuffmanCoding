#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>

class Element
{
public:
    virtual bool isLeaf() = 0;
    virtual ~Element(){}
};

class Leaf : public Element
{
public:
    Leaf(char _symbol) : symbol(_symbol),
        frequency(1){ }
    char symbol;
    int frequency;
    bool isLeaf() override { return true; }
};

class Node : public Element
{
public:
    Node():frequency(0), left(nullptr),
        right(nullptr){}
    int frequency;
    Element * left;
    Element * right;
    bool isLeaf() override { return false; }
};

decltype(auto)
create_queue(const std::string& s)
{
    std::vector<Element*> v;
    v.reserve(10);

    for(auto c : s)
    {
        if ( std::find_if(v.begin(), v.end(),
                          [c](Element* u) {
                               Leaf * l = dynamic_cast<Leaf*>(u);
                               if (l->symbol == c) {
                                    l->frequency++;
                                    return true;
                               }
                               return false;
                          } ) == v.end() )
        {
            v.push_back(new Leaf(c));
        }
    }

    auto comp = [] (const Element *a, const Element *b)
    { return ((const Leaf *)a)->frequency > ((const Leaf *)b)->frequency; };

    using queue_t = std::priority_queue<Element*, std::vector<Element*>, decltype(comp)>;

    auto delEl = [](queue_t * q)
    {
        while(!q->empty())
        {
            delete q->top();
            q->pop();
        }
        delete q;
    };

    std::unique_ptr<queue_t, decltype(delEl)> ptr(new queue_t(comp, v), delEl);
    return ptr;
}



int main()
{
    auto q = create_queue("abacabad");

//    while(!q->empty())
//    {
//        std::cout << ((Leaf*)q->top())->symbol << " " << ((Leaf*)q->top())->frequency << std::endl;
//        q->pop();
//    }

    return 0;
}
