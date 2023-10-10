#include <gtest/gtest.h>
#include <fstream>

class BoolTest : public ::testing::Test {
 protected:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

bool compare_files(const std::string& file1_path, const std::string& file2_path) {
    std::ifstream file1(file1_path);
    std::ifstream file2(file2_path);

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "Error: Could not open one or both of the files." << std::endl;
        return false;
    }

    char char1, char2;
    bool files_are_equal = true;

    while (true) {
        char1 = file1.get();
        char2 = file2.get();

        if (char1 != char2) {
            files_are_equal = false;
            break;
        }

        if (file1.eof() && file2.eof()) {
            break;  // Reached the end of both files
        }

        // If one file reaches the end before the other, they are not equal
        if (file1.eof() || file2.eof()) {
            files_are_equal = false;
            break;
        }
    }

    file1.close();
    file2.close();

    return files_are_equal;
}

TEST_F(BoolTest, ValueIsFalse) {
    std::string xml_input = "./test_cases/can_fade_tests/can_fade_is_false.xml";
    std::string xml_output = "./test_cases/test_output/can_fade_tests/can_fade_is_false.xml";
    EXPECT_TRUE(compare_files(xml_input, xml_output));
}

TEST_F(BoolTest, ValueIsZero) {
    std::string xml_input = "./test_cases/can_fade_tests/can_fade_is_zero.xml";
    std::string xml_output = "./test_cases/test_output/can_fade_tests/can_fade_is_zero.xml";
    EXPECT_FALSE(compare_files(xml_input, xml_output));
}
