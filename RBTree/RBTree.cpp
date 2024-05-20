#include <iostream>
#include "redblacktree.h"

int main()
{
    RedBlackTree<int> rb{};
    rb.insert(1);
    rb.insert(2);
    rb.insert(3);
    rb.insert(4);
    rb.insert(5);
    rb.insert(6);
    rb.insert(8);
    rb.insert(9);
    rb.insert(10);
    rb.remove(4);
    rb.print();
    return 0;
}
