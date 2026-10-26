#include "DynamicArray.h"
#include <iostream>
#include <utility>

DynamicArray::DynamicArray(const std::string& name, std::size_t size)
    : name_(name), size_(size), data_(new int[size])
{
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = 0;
    }
        std::cout << "[Constructor] '" << name_ << "' size=" << size_ << " heap=" << static_cast<const void*>(data_) << "\n";
}
DynamicArray::~DynamicArray() {
    std::cout << "[Destructor] '" << name_ << "' elibereaza memoria\n";
    delete[] data_;
}
DynamicArray::DynamicArray(const DynamicArray& other)
    : name_(other.name_ + "_copy"), size_(other.size_), data_(new int[other.size_])
{
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
    std::cout << "[Copy constructor] '" << name_ << "' are heap NOU "
              << static_cast<const void*>(data_) << " (sursa: "
              << static_cast<const void*>(other.data_) << ")\n";
}
DynamicArray::DynamicArray(DynamicArray&& other) noexcept
    : name_(std::move(other.name_)), size_(other.size_), data_(other.data_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    std::cout << "[Move constructor] '" << name_ << "' a preluat heap "
              << static_cast<const void*>(data_) << " (fara alocare noua)\n";
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