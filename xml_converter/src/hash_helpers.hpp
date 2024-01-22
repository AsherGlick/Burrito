#pragma once

#include <string>

#include "attribute/unique_id.hpp"

class Hash64 {
 private:
    uint64_t hash;

 public:
    Hash64();
    explicit Hash64(uint64_t init);
    void update(const unsigned char* str, size_t length);
    void update(const std::string& str);
    std::string hex() const;
};

class Hash128 {
 private:
    uint64_t upper;
    uint64_t lower;

 public:
    Hash128();
    Hash128(uint64_t upper, uint64_t lower);
    void update(const unsigned char* str, size_t length);
    void update(const std::string& str);
    std::string hex() const;
    UniqueId unique_id() const;
};
