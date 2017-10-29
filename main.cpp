#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>

using Unit = std::pair<char, int>;
using QueueContainer = std::vector<Unit>;

template<typename T>
using queue_t = std::priority_queue<Unit, QueueContainer, T>;

template<typename T>
using ptr_queue_t = std::unique_ptr<queue_t<T>>;

decltype(auto)
create_queue(const std::string& s)
{
    std::vector<Unit> v;
    v.reserve(10);

    for(auto c : s)
    {
        if ( std::find_if(v.begin(), v.end(),
                          [c](Unit& u) {
                               if (u.first == c) {
                                    u.second++;
                                    return true;
                               }
                               return false;
                          } ) == v.end() )
            v.push_back(Unit(c,1));
    }

    auto comp = [] (const Unit &a, const Unit &b)
    { return a.second > b.second; };

    auto ptr = ptr_queue_t<decltype(comp)>
            (new queue_t<decltype(comp)>(comp, std::move(v)));

    return ptr;
}

int main()
{
    auto q = create_queue("abacabad");

    while(!q->empty())
    {
        std::cout << q->top().first << " " << q->top().second << std::endl;
        q->pop();
    }

    return 0;
}
