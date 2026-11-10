

#include <iostream>           
#include "DynamicArray.h"     
#include "BadArray.h"         
#include <cstring>            
#include <utility>            
#include <vector>             


// CERINTA 3 Initializarea membrilor in constructor
// Member afiseaza un mesaj la fiecare operatie, ca sa vedem exact cate operatii se fac.

struct Member {
    // constructor implicit (fara parametri)
    Member()      { std::cout << "  [Member] constructor implicit\n"; }
    // constructor care primeste direct valoarea
    Member(int v) { std::cout << "  [Member] constructor cu valoarea " << v << "\n"; }
    // operator de atribuire: se apeleaza la "m_ = 5;"
    Member& operator=(int v) { std::cout << "  [Member] atribuire " << v << "\n"; return *this; }
};

// Initializare IN CORP: membrul se construieste mai intai implicit,
// apoi i se atribuie valoarea => DOI pasi (vezi output-ul)
struct InBody {
    Member m_;
    InBody() { m_ = 5; }          // doi pasi: implicit + atribuire
};

// Initializare IN LISTA: membrul se construieste direct cu 5 => UN singur pas
struct InList {
    Member m_;
    InList() : m_(5) {}           // un singur pas
};

// Cazul in care lista de initializare este OBLIGATORIE:
// un membru const nu poate fi atribuit in corp, doar initializat.
// E comentat pentru ca da eroare de compilare (dovada pentru cerinta 3).
// Ca sa vezi eroarea, scoate comentariul si ruleaza make.
/*
struct WithConst {
    const int id_;
    WithConst(int id) { id_ = id; }      // EROARE: id_ e const
    // varianta corecta: WithConst(int id) : id_(id) {}
};
*/


// ------------------------------------------------------------
// [CERINTA 4] Destructor si eliberarea heap-ului
// Obiectul tmp traieste doar in blocul { }. La acolada de inchidere
// se apeleaza automat destructorul, care face delete[] pe memoria alocata.
// ------------------------------------------------------------
void demoDestructor() {
    std::cout << "\nIntram in scope...\n";
    {   // incepe un scope interior
        DynamicArray tmp("temporar", 1000000);   // constructorul aloca ~4 MB pe heap
        std::cout << "Obiectul exista, ocupa ~4 MB pe heap\n";
    }   // aici dispare tmp => se apeleaza destructorul => memoria e eliberata
    std::cout << "Am iesit din scope\n";
}


// [CERINTA 5, partea 1] DE CE avem nevoie de copy constructor
// BadArray NU are copy constructor propriu, deci compilatorul
// copiaza doar pointerul (shallow copy). Rezultatul: doua obiecte
// folosesc aceeasi memorie si apare double free la destructori.
// Se ruleaza doar cu: ./tema_poo --crash

void demoBadCopy() {
    std::cout << "\n=== Copy constructor implicit (shallow copy) ===\n";
    BadArray original(3, 1);       // tablou de 3 elemente, toate 1
    BadArray copy = original;      // se apeleaza copy constructor-ul IMPLICIT

    // Adresele sunt identice => cele doua obiecte partajeaza aceeasi memorie
    std::cout << "adresa heap original: " << static_cast<const void*>(original.rawData()) << "\n";
    std::cout << "adresa heap copie   : " << static_cast<const void*>(copy.rawData()) << "\n";

    copy.set(0, 999);              // modificam DOAR copia...
    std::cout << "Am modificat DOAR copia: copy[0]=999\n";
    // ...dar si originalul s-a schimbat, pentru ca memoria e comuna
    std::cout << "original[0] = " << original.get(0) << "\n";

    // La acolada de inchidere se distrug copy si original. Ambele fac
    // delete[] pe aceeasi adresa => DOUBLE FREE => programul crapa.
}



// [CERINTA 5, partea 2] Solutia: copy constructor propriu (deep copy)
// DynamicArray are copy constructor care aloca memorie noua si
// copiaza elementele, deci fiecare obiect are heap-ul lui.

void demoGoodCopy() {
    std::cout << "\n=== Copy constructor propriu (deep copy) ===\n";
    DynamicArray original("original", 3);
    original.set(0, 1);                  // original = [1, 0, 0]
    DynamicArray copy = original;        // se apeleaza copy constructor-ul din DynamicArray

    copy.set(0, 999);                    // modificam DOAR copia
    std::cout << "Am modificat DOAR copia:\n";
    original.print();                    // ramane [1, 0, 0]
    copy.print();                        // [999, 0, 0]
    // La final fiecare obiect elibereaza propria memorie, fara crash.
}


// CERINTA 6 Move constructor
// Compara copierea (aloca memorie noua) cu mutarea (fura pointerul).

void demoMove() {
    std::cout << "\n=== Move constructor ===\n";
    DynamicArray source("source", 5);
    source.set(0, 7);

    std::cout << "\n-- Copiere (alocare noua + copiere elemente):\n";
    DynamicArray copied = source;        // copy constructor: adresa de heap NOUA

    std::cout << "\n-- Mutare (se fura pointerul, fara alocare):\n";
    // std::move il marcheaza pe source ca "poate fi golit" => se alege move constructor-ul.
    // moved preia aceeasi adresa de heap, iar source ramane gol (nullptr, size 0).
    DynamicArray moved = std::move(source);
    std::cout << "source dupa mutare: size=" << source.getSize() << "\n";   // 0
    moved.print();

    std::cout << "\n-- std::vector muta obiectele in loc sa le copieze:\n";
    std::vector<DynamicArray> v;
    v.reserve(2);                                // rezervam loc, ca sa nu se realoce
    v.push_back(DynamicArray("v1", 2));          // temporarul e MUTAT in vector
    v.push_back(DynamicArray("v2", 2));          // la fel
    // Liniile "[Destructor] ''" sunt destructorii temporarelor golite dupa mutare
    // (data_ == nullptr, deci delete[] nu face nimic).
}

int main(int argc, char *argv[]) {

    //CERINTA 3 Constructor cu lista de initializare (vezi DynamicArray.cpp)
    DynamicArray a("a", 3);

    //CERINTA 2 Incapsulare: accesam datele doar prin metode publice.
    // set() valideaza indexul si returneaza false daca e in afara tabloului.
    std::cout << "Scriem la index 1: " << (a.set(1, 42) ? "OK" : "respins") << "\n";
    std::cout << "Scriem la index 10: " << (a.set(10, 7) ? "OK" : "respins") << "\n";

    a.print();
    // CERINTA 2 Decomenteaza linia de mai jos ca sa vezi eroarea de compilare:
    // data_ este private, deci din exterior nu putem strica datele.
    // a.data_[10] = 5;

    //CERINTA 3 Initializare in corp vs in lista (compara mesajele din output)
    std::cout << "\nInitializare in corp:\n";
    InBody b;                    // doua operatii: implicit + atribuire
    std::cout << "Initializare in lista:\n";
    InList l;                    // o singura operatie

    //CERINTA 4 Destructorul elibereaza memoria la iesirea din scope
    demoDestructor();

    //CERINTA 5 Demo-ul cu crash ruleaza doar daca pornim: ./tema_poo --crash
    if (argc > 1 && std::strcmp(argv[1], "--crash") == 0) {
        demoBadCopy();           // se opreste aici cu double free
    }

    //CERINTA 5 Varianta corecta, cu copy constructor propriu
    demoGoodCopy();

    //CERINTA 6 Move constructor
    demoMove();

    return 0;
    // La return se distruge si obiectul 'a' (mesajul [Destructor] 'a' din final)
}