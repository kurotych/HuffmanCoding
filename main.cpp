#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <memory>

using Unit = std::pair<char, int>;
using QueueContainer = std::vector<Unit>;

template<typename T>
using queue_t = std::priority_queue<Unit, QueueContainer, T>;
template<typename T>
using ptr_queue_t = std::unique_ptr<queue_t<T>>;

template<typename T>
ptr_queue_t<T>
create_queue(T&& comp, const std::string& s)
{
    auto ptr = ptr_queue_t<T>(new queue_t<T>(std::forward<T>(comp)));

    ptr->push(Unit('a',4));
    ptr->push(Unit('b',2));
    ptr->push(Unit('c',3));

    return ptr;
}

int main()
{
    std::string str = "abacabad";

    auto comp = [] (const Unit &a, const Unit &b)
    { return a.second > b.second; };

    ptr_queue_t<decltype(comp)> q
            = create_queue<decltype(comp)>(std::move(comp), "abacabad");

    while(!q->empty())
    {
        std::cout << q->top().first << " " << q->top().second << std::endl;
        q->pop();
    }
    return 0;
}
