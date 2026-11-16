#pragma once
// Item 13: Use objects to manage resources
// Idee (RAII): pui resursa (memorie, fisier, mutex...) intr-un obiect.
// Destructorul obiectului elibereaza resursa AUTOMAT, indiferent
// cum se iese din functie (return, exceptie etc.).

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class Investment {
public:
    explicit Investment(const std::string& name) : name_(name) {
        std::cout << "  [+] Investment '" << name_ << "' creat\n";
    }
    ~Investment() {
        std::cout << "  [-] Investment '" << name_ << "' distrus\n";
    }
    void info() const { std::cout << "  folosesc '" << name_ << "'\n"; }

private:
    std::string name_;
};

// "Fabrica": returneaza pointer la un obiect creat cu new
inline Investment* createInvestment(const std::string& name) {
    return new Investment(name);
}

// GRESIT: pointer simplu, delete manual
inline void useRaw(bool earlyReturn) {
    Investment* p = createInvestment("Raw");
    p->info();
    if (earlyReturn) {
        std::cout << "  return timpuriu -> delete NU se mai executa (memory leak!)\n";
        return;
    }
    delete p;
}

// CORECT: std::unique_ptr (un singur proprietar)
inline void useUnique(bool earlyReturn) {
    std::unique_ptr<Investment> p(createInvestment("Unique"));
    p->info();
    if (earlyReturn) {
        std::cout << "  return timpuriu -> p isi elibereaza singur resursa\n";
        return;
    }
}

// CORECT si cand apare o exceptie
inline void useWithException() {
    std::unique_ptr<Investment> p(createInvestment("CuExceptie"));
    p->info();
    throw std::runtime_error("ceva a mers prost");
    // fara unique_ptr, un delete scris aici nu s-ar fi executat niciodata
}

inline void demoItem13() {
    std::cout << "=== Item 13: obiecte care gestioneaza resurse (RAII) ===\n";

    std::cout << "\n1) Pointer simplu, fara return timpuriu:\n";
    useRaw(false);

    std::cout << "\n2) Pointer simplu, CU return timpuriu (scurgere de memorie):\n";
    useRaw(true);                       // lipseste linia "[-] ... distrus"

    std::cout << "\n3) unique_ptr cu return timpuriu:\n";
    useUnique(true);                    // apare "[-] ... distrus"

    std::cout << "\n4) unique_ptr si exceptie:\n";
    try {
        useWithException();
    } catch (const std::exception& e) {
        std::cout << "  exceptie prinsa: " << e.what() << "\n";
    }

    std::cout << "\n5) unique_ptr NU se copiaza, doar se muta (transfer de proprietate):\n";
    std::unique_ptr<Investment> a = std::make_unique<Investment>("Mutat");
    // std::unique_ptr<Investment> b = a;       // NU compileaza
    std::unique_ptr<Investment> b = std::move(a);
    std::cout << "  a este " << (a ? "valid" : "nul")
              << ", b este " << (b ? "valid" : "nul") << "\n";
    b.reset();                          // distruge obiectul acum

    std::cout << "\n6) shared_ptr: mai multi proprietari, numarare de referinte:\n";
    std::shared_ptr<Investment> s1 = std::make_shared<Investment>("Shared");
    {
        std::shared_ptr<Investment> s2 = s1;    // copierea e permisa
        std::cout << "  use_count = " << s1.use_count() << "\n";
    }                                           // s2 iese din scope
    std::cout << "  use_count = " << s1.use_count() << " (obiectul inca exista)\n";
    s1.reset();                                 // ultimul proprietar -> se distruge

    std::cout << "\n";
}