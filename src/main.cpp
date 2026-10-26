#include <iostream>
#include "DynamicArray.h"

int main() {
    DynamicArray a("a", 3);

    std::cout << "Scriem la index 1: " << (a.set(1, 42) ? "OK" : "respins") << "\n";
    std::cout << "Scriem la index 10: " << (a.set(10, 7) ? "OK" : "respins") << "\n";

    a.print();
    return 0;
}