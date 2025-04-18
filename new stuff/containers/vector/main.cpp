#include <iostream>
#include "nvector.h"

int main()
{
    NVector<int> vec;
    int x = 0;
    vec.pushBack(x);
    vec.pushBack(2);
    vec.pushBack(3);
    vec.pushBack(4);
    return 0;
}
