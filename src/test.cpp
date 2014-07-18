#include "../include/puddle/puddle.hpp"
#include <list>
#include <iostream>

template <typename T>
using PuddleList = std::list<T, Puddle::Allocator<T>>;

int main()
{
    PuddleList<int> test;

    for (int i=0; i<40; ++i)
        test.emplace_back(i);

    for (int i : test)
        std::cout << i << std::endl;
}
