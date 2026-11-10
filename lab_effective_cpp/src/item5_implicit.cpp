#include "item5_implicit.hpp"
#include <algorithm>
#include <type_traits>
#include <utility>

namespace item5 {

// ---------- Tracer ----------
Tracer::Tracer(std::string name) : name_(std::move(name)) { log("constructor"); }
Tracer::Tracer(const Tracer& other) : name_(other.name_ + "_copie") { log("constructor de copiere"); }
Tracer& Tracer::operator=(const Tracer& other) {
    name_ = other.name_ + "_atribuit";
    log("operator= de copiere");
    return *this;
}
Tracer::~Tracer() { log("destructor"); }
void Tracer::log(const char* what) const {
    std::cout << "      [Tracer " << name_ << "] " << what << '\n';
}

// ---------- ShallowArray ----------
ShallowArray::ShallowArray(std::size_t n, int value)
    : size_(n), data_(new int[n]) {
    std::fill(data_, data_ + size_, value);
}

void ShallowArray::release() {
    delete[] data_;
    data_ = nullptr;
}

// ---------- DeepArray ----------
DeepArray::DeepArray(std::size_t n, int value)
    : size_(n), data_(new int[n]) {
    std::fill(data_, data_ + size_, value);
}

DeepArray::DeepArray(const DeepArray& other)
    : size_(other.size_), data_(new int[other.size_]) {
    std::copy(other.data_, other.data_ + size_, data_);
}

DeepArray::DeepArray(DeepArray&& other) noexcept
    : size_(other.size_), data_(other.data_) {
    other.size_ = 0;
    other.data_ = nullptr;
}

DeepArray& DeepArray::operator=(DeepArray rhs) noexcept {
    swap(rhs);              // rhs e o copie (sau mutare); la iesire o distruge
    return *this;
}

DeepArray::~DeepArray() { delete[] data_; }

void DeepArray::swap(DeepArray& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
}

// ---------- Demo ----------
void demo() {
    std::cout << "\n==================== ITEM 5 ====================\n";
    std::cout << std::boolalpha;

    std::cout << "\n-- 1) Clasa goala: ce genereaza compilatorul --\n";
    std::cout << "    sizeof(Empty)                    = " << sizeof(Empty) << "\n";
    std::cout << "    default constructible            = " << std::is_default_constructible<Empty>::value << "\n";
    std::cout << "    copy constructible               = " << std::is_copy_constructible<Empty>::value << "\n";
    std::cout << "    copy assignable                  = " << std::is_copy_assignable<Empty>::value << "\n";
    std::cout << "    destructible                     = " << std::is_destructible<Empty>::value << "\n";

    std::cout << "\n-- 2) Holder: functiile generate apeleaza functiile membrilor --\n";
    {
        std::cout << "    >> Holder h1;   (constructor implicit generat)\n";
        Holder h1;
        std::cout << "    >> Holder h2 = h1;   (constructor de copiere generat)\n";
        Holder h2 = h1;
        std::cout << "    >> Holder h3; h3 = h1;   (operator= generat)\n";
        Holder h3;
        h3 = h1;
        std::cout << "    >> iesire din scope (destructorii generati):\n";
    }

    std::cout << "\n-- 3) ShallowArray: copierea generata copiaza doar pointerul --\n";
    {
        ShallowArray a(3, 1);
        ShallowArray b = a;     // constructor de copiere generat
        b[0] = 99;
        std::cout << "    a[0] = " << a[0] << "  (modificat prin b!)\n";
        std::cout << "    a.address() == b.address() ? "
                  << (a.address() == b.address()) << "\n";
        a.release();            // eliberam o singura data; b ramane cu pointer invalid
    }

    std::cout << "\n-- 4) DeepArray: copiere profunda scrisa de noi --\n";
    {
        DeepArray a(3, 1);
        DeepArray b = a;        // constructor de copiere propriu
        b[0] = 99;
        std::cout << "    a[0] = " << a[0] << ", b[0] = " << b[0] << "\n";
        std::cout << "    a.address() == b.address() ? "
                  << (a.address() == b.address()) << "\n";

        DeepArray c(2, 5);
        c = a;                  // copy-and-swap
        DeepArray d = std::move(c);
        std::cout << "    dupa move: c.size() = " << c.size()
                  << ", d.size() = " << d.size() << "\n";
    }

    std::cout << "\n-- 5) Cand compilatorul REFUZA sa genereze operator= --\n";
    std::cout << "    Account copy constructible = " << std::is_copy_constructible<Account>::value << "\n";
    std::cout << "    Account copy assignable    = " << std::is_copy_assignable<Account>::value << "\n";
    // Daca ai incerca:  Account a1(1, n1), a2(2, n2);  a1 = a2;
    // => eroare de compilare: use of deleted function.
}

} // namespace item5