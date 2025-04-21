#include <iostream>
#include <list>
#include "nlist.h"

void bar();

int main()
{
    std::list<std::string> slist{"def1", "def2"};
    auto sit = slist.begin();
    slist.insert(sit, "new1");
    //sit = ++slist.begin();
    slist.insert(sit, "new2");
    slist.insert(sit, "new3");
    slist.insert(slist.end(), "new4");

    for (auto el : slist) {
        std::cout << el << "\n";
    }
    // std::cout << list.size() << "\n";
    // std::cout << list;

    std::cout << std::endl;

    NList<std::string> list{"def1", "def2"};
    auto it = list.begin();
    list.insert(it, "new1");
    //it = ++list.begin();
    list.insert(it, "new2");
    list.insert(it, "new3");
    list.insert(list.end(), "new4");
    // for (auto it = list.begin(); it != list.end(); ++it) {
    //     list.insert(it, "str" + std::to_string(i++));
    // }
    std::cout << list.size() << "\n";
    std::cout << list;
    return 0;
}
