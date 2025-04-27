#include <iostream>
#include <string>
#include "blocklist.h"

int main()
{
    BlockList<std::string> l;
    for (int i = 0; i < 20; ++i) {
         l.insertFront("str" + std::to_string(i + 1));
    }

    l.insert(0, "new0");
    l.insert(1, "new1");
    l.insert(1, "new2");
    l.insert(2, "new3");
    l.insert(5, "new5");
    l.insert(15, "new15");
    l.insert(10, "new10");

    for (int i = 0; i < l.size(); ++i) {
        std::cout << l[i] << "\n";
    }
    return 0;
}
