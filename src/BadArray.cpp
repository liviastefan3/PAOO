#include "BadArray.h"
#include <iostream>

BadArray::BadArray(std::size_t size, int initialValue)
    : size_(size), data_(new int[size])
{
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = initialValue;
    }
}

BadArray::~BadArray() {
    std::cout << "[BadArray destructor] delete[] pe "
              << static_cast<const void*>(data_) << "\n";
    delete[] data_;
}

void BadArray::set(std::size_t index, int value) {
    if (index < size_) data_[index] = value;
}

int BadArray::get(std::size_t index) const {
    return index < size_ ? data_[index] : -1;
}

const int* BadArray::rawData() const {
    return data_;
}