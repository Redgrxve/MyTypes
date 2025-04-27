#include <iostream>
#include <string>
#include "blocklist.h"

template <typename T>
void rebuildTableTest(const BlockList<T> &l) {
    std::cout << "Before rebuild: \n";
    l.printIndexTable();
    std::cout << std::endl;
    for (int i = 0; i < l.size(); ++i) {
        std::cout << l[i] << "\n";
    }

    l.rebuildIndexTable();
    std::cout << "\nAfter rebuild: \n";
    l.printIndexTable();
    std::cout << std::endl;
    for (int i = 0; i < l.size(); ++i) {
        std::cout << l[i] << "\n";
    }
}

int main()
{
    BlockList<std::string> l;
    for (int i = 0; i < 10; ++i) {
         l.insertBack("str" + std::to_string(i + 1));
    }

    // l.insert(0, "new0");
    // l.insert(1, "new1");
    // l.insert(1, "new2");
    // l.insert(2, "new3");
    // l.insert(5, "new5");
    // l.insert(15, "new15");
    // l.insert(10, "new10");

    std::cout << "Before: \n";
    for (int i = 0; i < l.size(); ++i) {
        std::cout << l[i] << "\n";
    }

    l.erase(0);
    l.erase(3);
    l.erase(5);
    //l.erase(7);
    l.erase(0);

    std::cout << "\nAfter: \n";
    for (int i = 0; i < l.size(); ++i) {
        std::cout << l[i] << "\n";
    }


    return 0;
}


