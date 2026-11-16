#pragma once
// Item 11: Handle assignment to self in operator=
// Problema: a = a;  Daca operator= sterge memoria proprie inainte
// sa copieze, se sterge si sursa (e acelasi obiect!).

#include <algorithm>
#include <iostream>
#include <utility>

// Varianta GRESITA (NU o apela cu a = a, e comportament nedefinit)
class BufferUnsafe {
public:
    explicit BufferUnsafe(int size = 3, int fill = 0)
        : data_(new int[size]), size_(size) {
        std::fill(data_, data_ + size_, fill);
    }
    BufferUnsafe(const BufferUnsafe& o) : data_(new int[o.size_]), size_(o.size_) {
        std::copy(o.data_, o.data_ + size_, data_);
    }
    ~BufferUnsafe() { delete[] data_; }

    BufferUnsafe& operator=(const BufferUnsafe& rhs) {
        delete[] data_;                    // daca this == &rhs, am sters si sursa!
        data_ = new int[rhs.size_];
        size_ = rhs.size_;
        std::copy(rhs.data_, rhs.data_ + size_, data_);  // citim memorie stearsa
        return *this;
    }

private:
    int* data_;
    int size_;
};

// Solutia 1: "identity test" + new inainte de delete
class BufferIdentity {
public:
    explicit BufferIdentity(int size = 3, int fill = 0)
        : data_(new int[size]), size_(size) {
        std::fill(data_, data_ + size_, fill);
    }
    BufferIdentity(const BufferIdentity& o) : data_(new int[o.size_]), size_(o.size_) {
        std::copy(o.data_, o.data_ + size_, data_);
    }
    ~BufferIdentity() { delete[] data_; }

    BufferIdentity& operator=(const BufferIdentity& rhs) {
        if (this == &rhs) return *this;    // testul de identitate

        // Alocam mai intai: daca new arunca exceptie, obiectul ramane neschimbat
        int* newData = new int[rhs.size_];
        std::copy(rhs.data_, rhs.data_ + rhs.size_, newData);

        delete[] data_;                    // abia acum stergem vechea memorie
        data_ = newData;
        size_ = rhs.size_;
        return *this;
    }

    int first() const { return data_[0]; }
    int size() const { return size_; }

private:
    int* data_;
    int size_;
};

// Solutia 2: copy-and-swap
class BufferSwap {
public:
    explicit BufferSwap(int size = 3, int fill = 0)
        : data_(new int[size]), size_(size) {
        std::fill(data_, data_ + size_, fill);
    }
    BufferSwap(const BufferSwap& o) : data_(new int[o.size_]), size_(o.size_) {
        std::copy(o.data_, o.data_ + size_, data_);
    }
    ~BufferSwap() { delete[] data_; }

    void swap(BufferSwap& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    // rhs e luat PRIN VALOARE => e deja o copie. Facem swap cu copia.
    BufferSwap& operator=(BufferSwap rhs) {
        swap(rhs);
        return *this;                      // copia (cu vechile date) se distruge singura
    }

    int first() const { return data_[0]; }
    int size() const { return size_; }

private:
    int* data_;
    int size_;
};

inline void demoItem11() {
    std::cout << "=== Item 11: auto-atribuirea (a = a) ===\n";

    BufferIdentity a(4, 42);
    a = a;                                 // sigur datorita testului de identitate
    std::cout << "BufferIdentity dupa a = a: first=" << a.first()
              << " size=" << a.size() << "\n";

    BufferIdentity b(2, 7);
    a = b;                                 // atribuire normala
    std::cout << "BufferIdentity dupa a = b: first=" << a.first()
              << " size=" << a.size() << "\n";

    BufferSwap c(4, 99);
    c = c;                                 // sigur si cu copy-and-swap
    std::cout << "BufferSwap dupa c = c: first=" << c.first()
              << " size=" << c.size() << "\n";

    BufferSwap d(2, 5);
    c = d;
    std::cout << "BufferSwap dupa c = d: first=" << c.first()
              << " size=" << c.size() << "\n";

    // BufferUnsafe u; u = u;   // <-- NU face asta: comportament nedefinit
    std::cout << "(BufferUnsafe exista doar ca exemplu de cod gresit)\n\n";
}