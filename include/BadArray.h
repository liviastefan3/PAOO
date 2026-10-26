#ifndef BAD_ARRAY_H
#define BAD_ARRAY_H

#include <cstddef>

// Clasa GRESITA, doar pentru demonstratie: are destructor, dar NU are copy constructor.
class BadArray {
private:
    std::size_t size_;
    int* data_;

public:
    BadArray(std::size_t size, int initialValue);
    ~BadArray();

    void set(std::size_t index, int value);
    int get(std::size_t index) const;
    const int* rawData() const;
};

#endif