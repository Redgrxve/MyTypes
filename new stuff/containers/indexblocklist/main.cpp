#include <iostream>
#include <list>
#include <array>
#include <deque>
#include "blocklist.h"

void bar();

int main()
{
    // std::array<int, 5> arr;
    // for (int i = 0; i < 5; ++i) {
    //     arr[i] = i + 1;
    // }

    // for (int i = 4; i > 2; --i)
    //     arr[i] = std::move_if_noexcept(arr[i - 1]);
    // arr[2] = 0;

    // for (int i = 0; i < 5; ++i) {
    //     std::cout << arr[i];
    // }

    BlockList<std::string> l;
    for (int i = 0; i < 20; ++i) {
        l.insertFront("str" + std::to_string(i + 1));
    }

    l.insert(1, "new1");
    l.insert(12, "new2");
    l.insert(10, "new3");

    l.insert(10, "new4");
    l.insert(15, "new5");
    //l.insert(1, "new3");


    for (int i = 0; i < l.size(); ++i) {
        std::cout << l[i] << "\n";
    }

    // std::cout << std::endl;

    // BlockList<std::string> l2(std::move(l));
    // for (int i = 0; i < 20; ++i) {
    //     std::cout << l2[i] << "\n";
    // }
    return 0;
}
