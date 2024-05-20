#include <iostream>
#include <string>
#include <set>
#include "redblacktree.h"


int main()
{
    RedBlackTree<int> rb{ 1, 2 };
    rb.print();
    std::cout << '\n';
    rb.remove(1);
    rb.insert(3);
    rb.insert(4);
    rb.insert(5);
    rb.remove(3);

    rb.print();
    std::cout << '\n';
    return 0;
}
