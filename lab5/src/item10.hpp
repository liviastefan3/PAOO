#pragma once
// Item 10: Have assignment operators return a reference to *this
// Regula: operator= (si +=, -=, etc.) returneaza Class& si face "return *this;"
// Asa merge si inlantuirea: a = b = c;

#include <iostream>

// Varianta CORECTA
class Counter {
public:
    explicit Counter(int v = 0) : value_(v) {}

    Counter& operator=(const Counter& rhs) {
        value_ = rhs.value_;
        return *this;               // <-- esenta Item 10
    }

    Counter& operator+=(const Counter& rhs) {
        value_ += rhs.value_;
        return *this;               // la fel si pentru +=
    }

    int value() const { return value_; }

private:
    int value_;
};

// Varianta GRESITA: operator= returneaza void
class CounterBad {
public:
    explicit CounterBad(int v = 0) : value_(v) {}

    void operator=(const CounterBad& rhs) {   // <-- nu returneaza nimic
        value_ = rhs.value_;
    }

    int value() const { return value_; }

private:
    int value_;
};

inline void demoItem10() {
    std::cout << "=== Item 10: operator= returneaza *this ===\n";

    Counter a(1), b(2), c(5);
    a = b = c;                      // se evalueaza ca: a = (b = c)
    std::cout << "dupa a = b = c: a=" << a.value()
              << " b=" << b.value() << " c=" << c.value() << "\n";

    (a += b) += c;                  // merge doar pentru ca += returneaza *this
    std::cout << "dupa (a += b) += c: a=" << a.value() << "\n";

    CounterBad x(1), y(2), z(7);
    x = y;                          // o atribuire simpla merge
    // x = y = z;                   // <-- NU compileaza! (y = z) e void
    (void)z;
    std::cout << "CounterBad: x = y merge, dar x = y = z nu compileaza"
              << " (scoate comentariul ca sa vezi eroarea)\n\n";
}