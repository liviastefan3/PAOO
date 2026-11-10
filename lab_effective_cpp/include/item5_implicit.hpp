#pragma once
#include <cstddef>
#include <iostream>
#include <string>

namespace item5 {

// Clasa goala: compilatorul ii genereaza constructor implicit,
// destructor, constructor de copiere si operator= de copiere.
class Empty {};

// Clasa care isi anunta fiecare apel (ca sa vedem ce apeleaza
// functiile generate de compilator pentru clasa Holder).
class Tracer {
public:
    explicit Tracer(std::string name);
    Tracer(const Tracer& other);
    Tracer& operator=(const Tracer& other);
    ~Tracer();
private:
    void log(const char* what) const;
    std::string name_;
};

// Holder nu declara NICIO functie speciala: le genereaza compilatorul,
// iar acestea apeleaza functiile membrilor.
class Holder {
    Tracer first_{"first"};
    Tracer second_{"second"};
};

// Pointer "owning" fara destructor/copy ctor/operator= definite:
// compilatorul copiaza pointerul (copiere superficiala).
class ShallowArray {
public:
    ShallowArray(std::size_t n, int value);
    int& operator[](std::size_t i) { return data_[i]; }
    const int* address() const { return data_; }
    void release();   // eliberare manuala (nu avem destructor)
private:
    std::size_t size_;
    int*        data_;
};

// Varianta corecta: Rule of Five definita explicit (copy-and-swap).
class DeepArray {
public:
    DeepArray(std::size_t n, int value);
    DeepArray(const DeepArray& other);               // copiere profunda
    DeepArray(DeepArray&& other) noexcept;           // mutare
    DeepArray& operator=(DeepArray rhs) noexcept;    // copy-and-swap
    ~DeepArray();

    int& operator[](std::size_t i) { return data_[i]; }
    const int* address() const { return data_; }
    std::size_t size() const { return size_; }
    void swap(DeepArray& other) noexcept;
private:
    std::size_t size_;
    int*        data_;
};

// Membru const + membru referinta => compilatorul NU poate genera
// operator= de copiere (ar fi implicit sters).
class Account {
public:
    Account(int id, std::string& owner) : id_(id), owner_(owner) {}
private:
    const int    id_;
    std::string& owner_;
};

void demo();

} // namespace item5