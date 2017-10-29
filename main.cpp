#include <iostream>
#include <queue>
#include <vector>
#include <memory>

using Unit = std::pair<char, int>;
using QueueContainer = std::vector<Unit>;

template<typename T>
using queue_t = std::priority_queue<Unit, QueueContainer, T>;
template<typename T>
using ptr_queue_t = std::unique_ptr<queue_t<T>>;

decltype(auto)
create_queue(const std::string& s)
{
    auto comp = [] (const Unit &a, const Unit &b)
    { return a.second > b.second; };

    auto ptr = ptr_queue_t<decltype(comp)>(new queue_t<decltype(comp)>(comp));

    ptr->push(Unit('a',4));
    ptr->push(Unit('b',2));
    ptr->push(Unit('c',3));

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
