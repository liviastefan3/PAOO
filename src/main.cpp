#include <iostream>
#include "DynamicArray.h"
struct Member {
    Member()      { std::cout << "  [Member] constructor implicit\n"; }
    Member(int v) { std::cout << "  [Member] constructor cu valoarea " << v << "\n"; }
    Member& operator=(int v) { std::cout << "  [Member] atribuire " << v << "\n"; return *this; }
};

struct InBody {
    Member m_;
    InBody() { m_ = 5; }          // doi pasi: implicit + atribuire
};

struct InList {
    Member m_;
    InList() : m_(5) {}           // un singur pas
};
/** da eroare pt cerinta 3
 
 * struct WithConst {
    const int id_;
    WithConst(int id) { id_ = id; }
};
 */
int main() {
    DynamicArray a("a", 3);

    std::cout << "Scriem la index 1: " << (a.set(1, 42) ? "OK" : "respins") << "\n";
    std::cout << "Scriem la index 10: " << (a.set(10, 7) ? "OK" : "respins") << "\n";

    a.print();
    //a.data_[10] = 5; - o sa dea eroare pt cerinta 2 cand compilam, care arata ca din exterior nu putem strica datele
    std::cout << "\nInitializare in corp:\n";
    InBody b;
    std::cout << "Initializare in lista:\n";
    InList l;
    return 0;
}