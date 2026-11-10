#include "DynamicArray.h"
#include <iostream>
#include <utility>     
//cmake e un instr care genereaza fis de build, fis de build iau toate fis de cod 
//si le gtransforma intr un singur cod sursa pe care i l dau la comp sa l compileze

// ===== CERINTA 3 CONSTRUCTOR =====
// Lista de initializare (dupa ':') construieste membrii direct cu valoarea finala.
// data_(new int[size]) aloca memorie pe heap.
DynamicArray::DynamicArray(const std::string& name, std::size_t size)
    : name_(name), size_(size), data_(new int[size])
{
    // punem 0 in toate elementele
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = 0;
    }
    std::cout << "[Constructor] '" << name_ << "' size=" << size_
              << " heap=" << static_cast<const void*>(data_) << "\n";
}

// ===== CERINTA 4 DESTRUCTOR =====
// Apelat automat cand obiectul dispare. Elibereaza memoria alocata cu new[].
DynamicArray::~DynamicArray() {
    std::cout << "[Destructor] '" << name_ << "' elibereaza memoria\n";
    delete[] data_;     // pe nullptr (obiect mutat) nu face nimic
}

// =====CERINTA 5 COPY CONSTRUCTOR (deep copy) =====
// Aloca memorie NOUA si copiaza elementele, nu doar pointerul.
// Sursa vine ca referinta const: nu se copiaza si nu se modifica.
DynamicArray::DynamicArray(const DynamicArray& other)
    : name_(other.name_ + "_copy"), size_(other.size_), data_(new int[other.size_])
{
    // copiem fiecare element din sursa in memoria noua
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
    std::cout << "[Copy constructor] '" << name_ << "' are heap NOU "
              << static_cast<const void*>(data_) << " (sursa: "
              << static_cast<const void*>(other.data_) << ")\n";
}

// ===== CERINTA 6 MOVE CONSTRUCTOR =====
// Fura pointerul sursei, fara alocare noua.
// noexcept = promite ca nu arunca exceptii (ca std::vector sa mute, nu sa copieze).
DynamicArray::DynamicArray(DynamicArray&& other) noexcept
    : name_(std::move(other.name_)), size_(other.size_), data_(other.data_)
{
    other.data_ = nullptr;   // sursa nu mai detine memoria (evita double free)
    other.size_ = 0;         // sursa ramane goala, dar valida
    std::cout << "[Move constructor] '" << name_ << "' a preluat heap "
              << static_cast<const void*>(data_) << " (fara alocare noua)\n";
}

// ===== CERINTA 2 METODE PUBLICE (incapsulare) =====

// returneaza numele (doar citire)
const std::string& DynamicArray::getName() const {
    return name_;
}

// returneaza dimensiunea (doar citire)
std::size_t DynamicArray::getSize() const {
    return size_;
}

// scrie o valoare, dar doar daca indexul e valid (validarea protejeaza datele)
bool DynamicArray::set(std::size_t index, int value) {
    if (index >= size_) {
        return false;        // index in afara tabloului: respins
    }
    data_[index] = value;
    return true;
}

// citeste o valoare; rezultatul ajunge in 'out' (transmis prin referinta)
bool DynamicArray::get(std::size_t index, int& out) const {
    if (index >= size_) {
        return false;
    }
    out = data_[index];
    return true;
}

// afiseaza tabloul sub forma: nume = [a, b, c]
void DynamicArray::print() const {
    std::cout << name_ << " = [";
    for (std::size_t i = 0; i < size_; ++i) {
        std::cout << data_[i];
        if (i + 1 < size_) std::cout << ", ";   // virgula intre elemente
    }
    std::cout << "]\n";
}