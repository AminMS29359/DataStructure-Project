#pragma once
#include <iostream>
#include <string>

template<typename T>
class hash {
public:
    size_t operator()(const T& key) const {
        return static_cast<size_t>(key);
    }
};

template<>
class hash<std::string> {
public:
    size_t operator()(const std::string& key) const {
        size_t value = 0;
        for (char c : key)
            value = value * 37 + c;
        return value;
    }
};