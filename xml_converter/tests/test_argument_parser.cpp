#include "../src/argument_parser.hpp"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>


struct PathConfigTestParams {
    BehaviorType type;
    MarkerFormat format;
    std::string path;
    bool split_by_map_id;
};

class PathConfigTest : public ::testing::TestWithParam<PathConfigTestParams> {
protected:
    PathConfig path_config;

    void SetUp() override {
        const auto& params = GetParam();
        path_config = PathConfig(params.type, params.format, params.path, params.split_by_map_id);
    }
};

TEST_P(PathConfigTest, BasicConstruction) {
    const auto& params = GetParam();
    EXPECT_EQ(path_config.type, params.type);
    EXPECT_EQ(path_config.format, params.format);
    EXPECT_EQ(path_config.path, params.path);
    EXPECT_EQ(path_config.split_by_map_id, params.split_by_map_id);
}

INSTANTIATE_TEST_SUITE_P(
    PathConfigTestCases,
    PathConfigTest,
    ::testing::Values(
        PathConfigTestParams{BehaviorType::IMPORT, MarkerFormat::XML, "path1", false},
        PathConfigTestParams{BehaviorType::IMPORT, MarkerFormat::XML, "path2", true},
        PathConfigTestParams{BehaviorType::IMPORT, MarkerFormat::GUILDPOINT, "path1", false},
        PathConfigTestParams{BehaviorType::IMPORT, MarkerFormat::GUILDPOINT, "path2", true},
        PathConfigTestParams{BehaviorType::EXPORT, MarkerFormat::XML, "path1", false},
        PathConfigTestParams{BehaviorType::EXPORT, MarkerFormat::XML, "path2", true},
        PathConfigTestParams{BehaviorType::EXPORT, MarkerFormat::GUILDPOINT, "path1", false},
        PathConfigTestParams{BehaviorType::EXPORT, MarkerFormat::GUILDPOINT, "path2", true}
        )
    );

class ParseArgumentsTest : public ::testing::Test {};

TEST_F(ParseArgumentsTest, ValidInputPaths){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--input-taco-path",
        (char*)"input1",
        (char*)"--output-taco-path",
        (char*)"output1"
    };
    int argc = sizeof(argv) / sizeof(char*);

    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    EXPECT_FALSE(parsed_arguments.allow_duplicates);

    ASSERT_EQ(parsed_arguments.path_configs.size(), 2);

    EXPECT_EQ(parsed_arguments.path_configs[0].type, BehaviorType::IMPORT);
    EXPECT_EQ(parsed_arguments.path_configs[0].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[0].path, "input1");
    EXPECT_FALSE(parsed_arguments.path_configs[0].split_by_map_id);

    EXPECT_EQ(parsed_arguments.path_configs[1].type, BehaviorType::EXPORT);
    EXPECT_EQ(parsed_arguments.path_configs[1].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[1].path, "output1");
    EXPECT_FALSE(parsed_arguments.path_configs[1].split_by_map_id);
}

TEST_F(ParseArgumentsTest, ValidFlagOrder){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--input-taco-path",
        (char*)"input1",
        (char*)"--output-taco-path",
        (char*)"--split-by-map-id",
        (char*)"output1"
    };
    int argc = sizeof(argv) / sizeof(char*);

    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    EXPECT_FALSE(parsed_arguments.allow_duplicates);

    ASSERT_EQ(parsed_arguments.path_configs.size(), 2);

    EXPECT_EQ(parsed_arguments.path_configs[0].type, BehaviorType::IMPORT);
    EXPECT_EQ(parsed_arguments.path_configs[0].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[0].path, "input1");
    EXPECT_FALSE(parsed_arguments.path_configs[0].split_by_map_id);

    EXPECT_EQ(parsed_arguments.path_configs[1].type, BehaviorType::EXPORT);
    EXPECT_EQ(parsed_arguments.path_configs[1].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[1].path, "output1");
    EXPECT_TRUE(parsed_arguments.path_configs[1].split_by_map_id);
}

TEST_F(ParseArgumentsTest, ValidFlagOrder2){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--input-taco-path",
        (char*)"input1",
        (char*)"--output-taco-path",
        (char*)"output1",
        (char*)"--split-by-map-id",
    };
    int argc = sizeof(argv) / sizeof(char*);

    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    EXPECT_FALSE(parsed_arguments.allow_duplicates);

    ASSERT_EQ(parsed_arguments.path_configs.size(), 2);

    EXPECT_EQ(parsed_arguments.path_configs[0].type, BehaviorType::IMPORT);
    EXPECT_EQ(parsed_arguments.path_configs[0].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[0].path, "input1");
    EXPECT_FALSE(parsed_arguments.path_configs[0].split_by_map_id);

    EXPECT_EQ(parsed_arguments.path_configs[1].type, BehaviorType::EXPORT);
    EXPECT_EQ(parsed_arguments.path_configs[1].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[1].path, "output1");
    EXPECT_TRUE(parsed_arguments.path_configs[1].split_by_map_id);
}

TEST_F(ParseArgumentsTest, ValidMultipleInputPaths){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--allow-duplicates",
        (char*)"--input-taco-path",
        (char*)"input1",
        (char*)"input2",
        (char*)"input3",
        (char*)"--output-guildpoint-path",
        (char*)"output1",
        (char*)"--split-by-map-id"
    };
    int argc = sizeof(argv) / sizeof(char*);

    ParsedArguments parsed_arguments = parse_arguments(argc, argv);

    ASSERT_EQ(parsed_arguments.path_configs.size(), 4);
    EXPECT_TRUE(parsed_arguments.allow_duplicates);

    EXPECT_EQ(parsed_arguments.path_configs[0].type, BehaviorType::IMPORT);
    EXPECT_EQ(parsed_arguments.path_configs[0].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[0].path, "input1");
    EXPECT_FALSE(parsed_arguments.path_configs[0].split_by_map_id);

    EXPECT_EQ(parsed_arguments.path_configs[1].type, BehaviorType::IMPORT);
    EXPECT_EQ(parsed_arguments.path_configs[1].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[1].path, "input2");
    EXPECT_FALSE(parsed_arguments.path_configs[1].split_by_map_id);

    EXPECT_EQ(parsed_arguments.path_configs[2].type, BehaviorType::IMPORT);
    EXPECT_EQ(parsed_arguments.path_configs[2].format, MarkerFormat::XML);
    EXPECT_EQ(parsed_arguments.path_configs[2].path, "input3");
    EXPECT_FALSE(parsed_arguments.path_configs[2].split_by_map_id);

    EXPECT_EQ(parsed_arguments.path_configs[3].type, BehaviorType::EXPORT);
    EXPECT_EQ(parsed_arguments.path_configs[3].format, MarkerFormat::GUILDPOINT);
    EXPECT_EQ(parsed_arguments.path_configs[3].path, "output1");
    EXPECT_TRUE(parsed_arguments.path_configs[3].split_by_map_id);
}


TEST_F(ParseArgumentsTest, InvalidSplitMapIDAfterInput){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--input-taco-path",
        (char*)"input1",
        (char*)"--split-by-map-id",
        (char*)"--output-taco-path",
        (char*)"output1"
    };
    int argc = sizeof(argv) / sizeof(char*);

    testing::internal::CaptureStderr();
    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    std::string std_err = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(parsed_arguments.path_configs.empty());
    EXPECT_NE(std_err.find("Error: --split-by-map-id cannot be used after an input argument"), std::string::npos);
}

TEST_F(ParseArgumentsTest, InvalidNoPath){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--input-taco-path",
        (char*)"--output-taco-path",
        (char*)"output1"
    };
    int argc = sizeof(argv) / sizeof(char*);

    testing::internal::CaptureStderr();
    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    std::string std_err = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(parsed_arguments.path_configs.empty());
    EXPECT_NE(std_err.find("Error: Expected a path to a directory after --input-taco-path"), std::string::npos);
}

TEST_F(ParseArgumentsTest, InvalidNoPath2){
    char* argv[] = {
        (char*)"./xml_converter",
        (char*)"--input-taco-path",
        (char*)"input1",
        (char*)"--output-taco-path"
    };
    int argc = sizeof(argv) / sizeof(char*);

    testing::internal::CaptureStderr();
    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    std::string std_err = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(parsed_arguments.path_configs.empty());
    EXPECT_NE(std_err.find("Error: Expected a path to a directory after --output-taco-path"), std::string::npos);
}
