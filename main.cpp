#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>
#include <list>
#include <map>

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
    Element * left = nullptr;
    Element * right = nullptr;
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

bool comp (const Element *a, const Element *b)
{ return a->frequency > b->frequency; };

bool (*comp_ptr) (const Element*,const Element*) = comp;
using queue_t = std::priority_queue<Element*, std::vector<Element*>, decltype(comp_ptr)>;

std::unique_ptr<queue_t>
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

    std::unique_ptr<queue_t> ptr(new queue_t(comp_ptr, v));
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

void free_tree(Element *n, std::map<char, std::string>& out, std::string s)
{
    if(n)
    {
        if(n->isLeaf())
        {
            if(s.empty())
                s = "0";
            out.insert(std::pair<char,std::string>( ((Leaf*)n)->symbol, s));
            return;
        }

        s.push_back('0');
        free_tree(((Node*)n)->left, out,s);
        s.pop_back();
        s.push_back('1');
        free_tree(((Node*)n)->right, out,s);
    }
    else
    {
        s.pop_back();
       // if(s.empty())
        //    s = "0";
       // out.insert(std::pair<char,std::string>( ((Leaf*)n)->symbol, s));

    }
    delete n;
}

std::string encrypt(const char *str, const std::map<char, std::string>& codes)
{
    std::string out_string;
    do {
        out_string += codes.at(*str);
    } while (*(++str));
    return out_string;
}
int main()
{
    char input_str[10000];
    std::cin >> input_str;
    auto q = create_queue(input_str);
    size_t s = q->size();
    std::map<char, std::string> codes;
    build_tree(q);

    free_tree(q->top(), codes, "");

    std::string result =  encrypt(input_str, codes);
    std::cout << s << " " << result.size() << std::endl;
    for(auto& s : codes)
        std::cout << s.first << ": " << s.second << std::endl;

    std::cout << result;
    return 0;
}
