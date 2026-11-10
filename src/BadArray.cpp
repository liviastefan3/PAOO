#include "BadArray.h"
#include <iostream>

// ===== CONSTRUCTOR =====
// Lista de initializare: size_ primeste dimensiunea, data_ primeste memorie noua
// alocata pe heap cu new[].
BadArray::BadArray(std::size_t size, int initialValue)
    : size_(size), data_(new int[size])
{
    // punem valoarea initiala in toate elementele
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = initialValue;
    }
}

// ===== DESTRUCTOR =====
// Elibereaza memoria. Problema apare la copiere: daca doua obiecte au aceeasi
// adresa in data_ (shallow copy), ambele destructoare ajung aici pe aceeasi
// memorie => al doilea delete[] da DOUBLE FREE si programul crapa.
BadArray::~BadArray() {
    std::cout << "[BadArray destructor] delete[] pe "
              << static_cast<const void*>(data_) << "\n";
    delete[] data_;
}

//METODE 

// scrie o valoare, doar daca indexul e valid
void BadArray::set(std::size_t index, int value) {
    if (index < size_) data_[index] = value;
}

// citeste o valoare; returneaza -1 daca indexul e invalid
int BadArray::get(std::size_t index) const {
    return index < size_ ? data_[index] : -1;
}

// returneaza adresa de heap; folosita doar in demo, ca sa aratam ca
// original si copie au ACEEASI adresa
const int* BadArray::rawData() const {
    return data_;
}

// ===== CE LIPSESTE INTENTIONAT =====
// Nu exista BadArray(const BadArray& other). De aceea compilatorul genereaza unul implicit, care copiaza doar pointerul (shallow copy).