#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>
#include <list>

class Element
{
public:
    Element(int freq): frequency(freq){}
    int frequency;
    virtual bool isLeaf() = 0;
    virtual ~Element(){}
};

class Leaf : public Element
{
public:
    Leaf(char _symbol): Element(1), symbol(_symbol){}
    char symbol;
    bool isLeaf() override { return true; }
};

class Node : public Element
{
public:
    Node(Element* a, Element* b)
        :Element(a->frequency + b->frequency), left(a), right(b){}
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
    { return a->frequency > b->frequency; };

    using queue_t = std::priority_queue<Element*, std::vector<Element*>, decltype(comp)>;

    std::unique_ptr<queue_t> ptr(new queue_t(comp, v));
    return ptr;
}


template<typename T> // T == queue_t
void build_tree(T& queue_ptr)
{
    while (queue_ptr->size() != 1) {
        Element * a = queue_ptr->top();
        queue_ptr->pop();
        Element * b = queue_ptr->top();
        queue_ptr->pop();
        queue_ptr->push(new Node(a,b));
    }
}

void free_tree(Element *n)
{
    if(!n->isLeaf())
    {
        free_tree(((Node*)n)->left);
        free_tree(((Node*)n)->right);
    }
    delete n;
}

int main()
{
    auto q = create_queue("abacabad");
    build_tree(q);
    free_tree(q->top());

//    while(!q->empty())
//    {
//        std::cout << ((Leaf*)q->top())->symbol << " " << ((Leaf*)q->top())->frequency << std::endl;
//        q->pop();
//    }

    return 0;
}
