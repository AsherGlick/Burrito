#include "../src/string_helper.hpp"
#include <gtest/gtest.h>

class Base64Test : public ::testing::Test {};

// Test Strip Bits

////////////////////////////////////////////////////////////////////////////////
// Test Double Strip Bits
//
// When there are `len%4 = 2` there are 4 extra bits stored in the base64 string
// This means that decoding and encoding that data will result in slightly
// different data bsecause those 4 extra bits get stripped off. This tests all
// of those different values so that we can be sure everything is happening
// deterministically.
////////////////////////////////////////////////////////////////////////////////
TEST_F(Base64Test, DoubleStripBits00) {
    // These are all of the base 64 characters which start with the binary
    // digits 00 and end with nonzero digits.
    std::string b64_00_characters = "BCDEFGHIJKLMNOP";

    for (size_t i = 0; i < b64_00_characters.size(); i++) {
        std::string input = "A_";
        input[1] = b64_00_characters[i];
        std::vector<uint8_t> decoded_value = base64_decode(input);
        std::string output = base64_encode(&decoded_value[0], decoded_value.size());
        EXPECT_EQ(output, "AA==");
    }
}
TEST_F(Base64Test, DoubleStripBits01) {
    // These are all of the base 64 characters which start with the binary
    // digits 01 and end with nonzero digits.
    std::string b64_00_characters = "RSTUVWXYZabcdef";

    for (size_t i = 0; i < b64_00_characters.size(); i++) {
        std::string input = "A_";
        input[1] = b64_00_characters[i];
        std::vector<uint8_t> decoded_value = base64_decode(input);
        std::string output = base64_encode(&decoded_value[0], decoded_value.size());
        EXPECT_EQ(output, "AQ==");
    }
}
TEST_F(Base64Test, DoubleStripBits10) {
    // These are all of the base 64 characters which start with the binary
    // digits 10 and end with nonzero digits.
    std::string b64_00_characters = "hijklmnopqrstuv";

    for (size_t i = 0; i < b64_00_characters.size(); i++) {
        std::string input = "A_";
        input[1] = b64_00_characters[i];
        std::vector<uint8_t> decoded_value = base64_decode(input);
        std::string output = base64_encode(&decoded_value[0], decoded_value.size());
        EXPECT_EQ(output, "Ag==");
    }
}
TEST_F(Base64Test, DoubleStripBits11) {
    // These are all of the base 64 characters which start with the binary
    // digits 11 and end with nonzero digits.
    std::string b64_00_characters = "xyz0123456789+/";
    for (size_t i = 0; i < b64_00_characters.size(); i++) {
        std::string input = "A_";
        input[1] = b64_00_characters[i];
        std::vector<uint8_t> decoded_value = base64_decode(input);
        std::string output = base64_encode(&decoded_value[0], decoded_value.size());
        EXPECT_EQ(output, "Aw==");
    }
}
