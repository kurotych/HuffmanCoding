#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

class Element
{
public:
    Element(int freq): frequency(freq){}
    int frequency;
    virtual bool isLeaf() const = 0;
    virtual ~Element(){}
};

class Leaf : public Element
{
public:
    Leaf(char _symbol): Element(1), symbol(_symbol){}
    char symbol;
    bool isLeaf() const override { return true; }
};

class Node : public Element
{
public:
    Node(Element* a, Element* b)
        :Element(a->frequency + b->frequency), left(a), right(b){}
    Element * left;
    Element * right;
    bool isLeaf() const override { return false; }
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

void free_tree(Element *n, std::map<char, std::string>& out, std::string s = "")
{
    if(n)
    {
        if(n->isLeaf())
        {
            if(s.empty())
                s = "0";
            out.insert(std::pair<char,std::string>( ((Leaf*)n)->symbol, s));
            delete n;
            return;
        }
        s.push_back('0');
        free_tree(((Node*)n)->left, out, s);
        s.pop_back();
        s.push_back('1');
        free_tree(((Node*)n)->right, out, s);
        delete n;
    }
    else
        s.pop_back();
}

std::string encrypt(const char *str, const std::map<char, std::string>& dict)
{
    std::string out_string;
    do {
        out_string += dict.at(*str);
    } while (*(++str));
    return out_string;
}

std::string decrypt(std::string input_str, const std::map<char, std::string>& dict)
{
    std::string result_str;

    while(!input_str.empty())
    {
        for(auto& d : dict)
        {
            if( d.second.length() <= input_str.length() )
            {
                if(input_str.substr(0, d.second.length()) == d.second )
                {
                    result_str += d.first;
                    input_str = std::move(input_str.substr(d.second.length()));
                }
            }
        }
    }

    return result_str;
}

int main()
{
    const char* input_str = "abacabad";
    auto q = create_queue(input_str);

    std::map<char, std::string> codes;
    build_tree(q);
    free_tree(q->top(), codes);

    std::string encStr = encrypt(std::move(input_str), codes);

    std::cout << "Encrypt str: " << encStr << std::endl;

    std::cout << "Decrypt str: " << decrypt(std::move(encStr), codes);

    return 0;
}
