#include "DynamicArray.h"
#include <iostream>

DynamicArray::DynamicArray(const std::string& name, std::size_t size) {
    name_ = name;
    size_ = size;
    data_ = new int[size_];
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = 0;
    }
    std::cout << "[Constructor] '" << name_ << "' size=" << size_ << "\n";
}

const std::string& DynamicArray::getName() const {
    return name_;
}

std::size_t DynamicArray::getSize() const {
    return size_;
}

bool DynamicArray::set(std::size_t index, int value) {
    if (index >= size_) {
        return false;
    }
    data_[index] = value;
    return true;
}

bool DynamicArray::get(std::size_t index, int& out) const {
    if (index >= size_) {
        return false;
    }
    out = data_[index];
    return true;
}

void DynamicArray::print() const {
    std::cout << name_ << " = [";
    for (std::size_t i = 0; i < size_; ++i) {
        std::cout << data_[i];
        if (i + 1 < size_) std::cout << ", ";
    }
    std::cout << "]\n";
}