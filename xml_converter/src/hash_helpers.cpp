// Hashing
#include "hash_helpers.hpp"

static const char* hex_chars = "0123456789abcdef";

Hash64::Hash64() {
    this->hash = 0b0000010010000001001101100111001011100111010110000100001101011110;
}

Hash64::Hash64(uint64_t init) {
    this->hash = init;
}

void Hash64::update(const unsigned char* str, size_t length) {
    for (size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }
}

void Hash64::update(const std::string& str) {
    this->update((unsigned char*)str.c_str(), str.length());
}

std::string Hash64::hex() const {
    std::string hex_string(16, '0');

    uint64_t number = this->hash;

    for (int i = 15; i >= 0; --i) {
        hex_string[i] = hex_chars[number & 0xF];
        number >>= 4;
    }

    return hex_string;
}

Hash128::Hash128() {
    this->lower = 0b0000010010000001001101100111001011100111010110000100001101011110;
    this->upper = 0b0101011000001100111101000001001100111111010110011101101111100100;
}
Hash128::Hash128(uint64_t upper, uint64_t lower) {
    this->upper = upper;
    this->lower = lower;
}

void Hash128::update(const unsigned char* str, size_t length) {
    for (size_t i = 0; i < length; i++) {
        upper = ((upper << 5) + upper) + ((lower >> 59) & 0x1F);
        uint64_t original_lower = lower;
        lower = lower << 5;

        uint64_t multiplication_overflow = (lower > UINT64_MAX - original_lower) ? 1 : 0;
        lower += original_lower;

        uint64_t addition_overflow = (lower > UINT64_MAX - str[i]) ? 1 : 0;

        lower += str[i];
        upper += multiplication_overflow + addition_overflow;
    }
}

void Hash128::update(const std::string& str) {
    this->update((unsigned char*)str.c_str(), str.length());
}

std::string Hash128::hex() const {
    std::string hex_string(32, '0');

    uint64_t number = this->lower;

    for (int i = 15; i >= 0; --i) {
        hex_string[i + 16] = hex_chars[number & 0xF];
        number >>= 4;
    }

    number = this->upper;
    for (int i = 15; i >= 0; --i) {
        hex_string[i] = hex_chars[number & 0xF];
        number >>= 4;
    }

    return hex_string;
}

Attribute::UniqueId::UniqueId Hash128::unique_id() const {
    Attribute::UniqueId::UniqueId unique_id;
    unique_id.guid = {
        (unsigned char)((this->upper >> 0) & 0xFF),
        (unsigned char)((this->upper >> 8) & 0xFF),
        (unsigned char)((this->upper >> 16) & 0xFF),
        (unsigned char)((this->upper >> 24) & 0xFF),
        (unsigned char)((this->lower >> 0) & 0xFF),
        (unsigned char)((this->lower >> 8) & 0xFF),
        (unsigned char)((this->lower >> 16) & 0xFF),
        (unsigned char)((this->lower >> 24) & 0xFF),
    };
    return unique_id;
}
