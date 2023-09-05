#include "cint.hpp"
#include "list2.hpp"
#include "vector2.hpp"

#include <iostream>
#include <vector>

using namespace std;
using namespace containers;

template <typename TContainer> void print(TContainer& v) {
    std::cout << "content: ";

    for (auto idx = v.begin(); idx != v.end(); ++idx) {
        std::cout << *idx << ", ";
    }

    std::cout << "size: " << v.size() << std::endl;
}

template <typename TContainer> void print_reverse(TContainer& v) {
    std::cout << "reverse content: ";

    auto idx = v.end();

    do {
        --idx;
        std::cout << *idx << ", ";
    } while (idx != v.begin());

    std::cout << "size: " << v.size() << std::endl;
}

template <typename TContainer, typename TItem>
void container_test(string TContainerName) {
    cout << "test: " << TContainerName << endl;

    TContainer v;

    for (int i = 0; i < 10; ++i) {
        v.push_back(TItem(i));
    }
    print(v);

    v.erase(6);
    v.erase(4);
    v.erase(2);
    print(v);

    v.insert(0, TItem(10));
    print(v);

    v.insert(4, TItem(20));
    print(v);

    auto v2 = v;
    print(v2);

    v2.push_back(TItem(30));
    auto v3 = std::move(v2);

    print(v3);

    print_reverse(v3);
}

template <typename TItem> void containers_test(string TItemName) {
    container_test<vector2<TItem>, TItem>("vector2<" + TItemName + ">");
    container_test<list2<TItem>, TItem>("list2<" + TItemName + ">");
}

void homework_test() {
    containers_test<int>("int");
}

int cint::instances;

void additional_test() {
    containers_test<cint>("int");
    std::cout << "cint::instances " << cint::instances << std::endl;
    assert(cint::instances == 0);
}

int main() {
    homework_test();
    // additional_test();

    return 0;
}
