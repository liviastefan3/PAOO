#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstddef>
#include <string>

class DynamicArray {
private:
    std::string name_;
    std::size_t size_;
    int* data_;

public:
    DynamicArray(const std::string& name, std::size_t size);
    ~DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    const std::string& getName() const;
    std::size_t getSize() const;
    bool set(std::size_t index, int value);
    bool get(std::size_t index, int& out) const;
    void print() const;
};

#endif