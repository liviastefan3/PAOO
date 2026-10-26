#include <iostream>
#include "DynamicArray.h"
#include "BadArray.h"
#include  <cstring>
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
void demoDestructor() {
    std::cout << "\nIntram in scope...\n";
    {
        DynamicArray tmp("temporar", 1000000);
        std::cout << "Obiectul exista, ocupa ~4 MB pe heap\n";
    }
    std::cout << "Am iesit din scope\n";
}
void demoBadCopy() {
    std::cout << "\n=== Copy constructor implicit (shallow copy) ===\n";
    BadArray original(3, 1);
    BadArray copy = original;

    std::cout << "adresa heap original: " << static_cast<const void*>(original.rawData()) << "\n";
    std::cout << "adresa heap copie   : " << static_cast<const void*>(copy.rawData()) << "\n";

    copy.set(0, 999);
    std::cout << "Am modificat DOAR copia: copy[0]=999\n";
    std::cout << "original[0] = " << original.get(0) << "\n";
}
void demoGoodCopy() {
    std::cout << "\n=== Copy constructor propriu (deep copy) ===\n";
    DynamicArray original("original", 3);
    original.set(0, 1);
    DynamicArray copy = original;

    copy.set(0, 999);
    std::cout << "Am modificat DOAR copia:\n";
    original.print();
    copy.print();
}
int main(int argc, char *argv[]) {
    DynamicArray a("a", 3);

    std::cout << "Scriem la index 1: " << (a.set(1, 42) ? "OK" : "respins") << "\n";
    std::cout << "Scriem la index 10: " << (a.set(10, 7) ? "OK" : "respins") << "\n";

    a.print();
    //a.data_[10] = 5; - o sa dea eroare pt cerinta 2 cand compilam, care arata ca din exterior nu putem strica datele
    std::cout << "\nInitializare in corp:\n";
    InBody b;
    std::cout << "Initializare in lista:\n";
    InList l;
    demoDestructor();
    if (argc > 1 && std::strcmp(argv[1], "--crash") == 0) {
        demoBadCopy();
    }
    //demoBadCopy();
    demoGoodCopy();
    return 0;
}