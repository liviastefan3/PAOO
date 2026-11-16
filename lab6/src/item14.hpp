#pragma once
// Item 14: Think carefully about copying behavior in resource-managing classes
// Cand scrii o clasa RAII trebuie sa decizi ce inseamna "copierea" ei.
// Variante:
//   1) Interzici copierea
//   2) Numaratoare de referinte (reference counting)
//   3) Copiezi resursa (deep copy)
//   4) Transferi proprietatea (move)

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// O "resursa" simpla: un mutex simulat (doar afiseaza ce se intampla)
class Mutex {
public:
    explicit Mutex(const std::string& name) : name_(name) {}
    void lock()   { std::cout << "    mutex '" << name_ << "' BLOCAT\n"; }
    void unlock() { std::cout << "    mutex '" << name_ << "' DEBLOCAT\n"; }
private:
    std::string name_;
};

inline void unlockMutex(Mutex* m) { m->unlock(); }

// ------------------------------------------------------------
// Varianta 1: interzicem copierea
// (are sens: nu are logica sa avem "doua lacate" pe acelasi mutex)
// ------------------------------------------------------------
class LockNoCopy {
public:
    explicit LockNoCopy(Mutex& m) : mutex_(m) { mutex_.lock(); }
    ~LockNoCopy() { mutex_.unlock(); }

    LockNoCopy(const LockNoCopy&) = delete;             // interzis
    LockNoCopy& operator=(const LockNoCopy&) = delete;  // interzis

private:
    Mutex& mutex_;
};

// ------------------------------------------------------------
// Varianta 2: reference counting cu shared_ptr si "deleter" propriu
// Mutex-ul se deblocheaza cand dispare ULTIMA copie, nu la prima.
// Copierea e generata corect de compilator.
// ------------------------------------------------------------
class LockShared {
public:
    explicit LockShared(Mutex* m) : ptr_(m, unlockMutex) { ptr_->lock(); }
    long count() const { return ptr_.use_count(); }
private:
    std::shared_ptr<Mutex> ptr_;    // la final apeleaza unlockMutex, nu delete
};

// ------------------------------------------------------------
// Varianta 3: copiem resursa (deep copy)
// Fiecare obiect are propria lui memorie.
// ------------------------------------------------------------
class Message {
public:
    explicit Message(const char* text)
        : text_(new char[std::strlen(text) + 1]) {
        std::strcpy(text_, text);
    }
    Message(const Message& rhs)                         // copiem CONTINUTUL
        : text_(new char[std::strlen(rhs.text_) + 1]) {
        std::strcpy(text_, rhs.text_);
    }
    Message& operator=(Message rhs) {                   // copy-and-swap (Item 11)
        std::swap(text_, rhs.text_);
        return *this;
    }
    ~Message() { delete[] text_; }

    const char* text() const { return text_; }
    const void* address() const { return text_; }

private:
    char* text_;
};

// ------------------------------------------------------------
// Varianta 4: transferam proprietatea (move)
// Copierea e interzisa automat (unique_ptr), dar mutarea e permisa.
// ------------------------------------------------------------
class LockMovable {
public:
    explicit LockMovable(Mutex* m) : ptr_(m, unlockMutex) { ptr_->lock(); }
    bool owns() const { return ptr_ != nullptr; }
private:
    std::unique_ptr<Mutex, void (*)(Mutex*)> ptr_;
};

inline void demoItem14() {
    std::cout << "=== Item 14: comportamentul la copiere in clase RAII ===\n";

    std::cout << "\n1) Copiere interzisa:\n";
    Mutex m1("m1");
    {
        LockNoCopy lock(m1);
        std::cout << "    in sectiunea critica\n";
        // LockNoCopy lock2(lock);       // NU compileaza (scoate // ca sa vezi)
    }

    std::cout << "\n2) Reference counting (shared_ptr cu deleter):\n";
    Mutex m2("m2");
    {
        LockShared a(&m2);
        {
            LockShared b(a);                            // copiem lacatul
            std::cout << "    count = " << a.count() << "\n";
        }
        std::cout << "    b distrus, count = " << a.count()
                  << " (mutexul inca blocat)\n";
    }                                                   // aici se deblocheaza

    std::cout << "\n3) Deep copy:\n";
    Message m3("salut");
    Message m4(m3);
    std::cout << "    m3 = '" << m3.text() << "', m4 = '" << m4.text() << "'\n";
    std::cout << "    adrese diferite: "
              << (m3.address() != m4.address() ? "da" : "nu") << "\n";

    std::cout << "\n4) Transfer de proprietate (move):\n";
    Mutex m5("m5");
    {
        LockMovable x(&m5);
        // LockMovable y(x);             // NU compileaza (copiere interzisa)
        LockMovable y(std::move(x));
        std::cout << "    x detine mutexul: " << (x.owns() ? "da" : "nu")
                  << ", y detine mutexul: " << (y.owns() ? "da" : "nu") << "\n";
    }                                                   // se deblocheaza O SINGURA data

    std::cout << "\n";
}