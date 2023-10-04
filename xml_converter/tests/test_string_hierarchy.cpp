#include "../src/string_hierarchy.hpp"
#include <gtest/gtest.h>


class StringHierarchyTest : public ::testing::Test {
 protected:
    StringHierarchy string_hierarchy;

    void SetUp() override {
    }
    void TearDown() override {
    }
};


TEST_F(StringHierarchyTest, BasicPath) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "child1", "child2"}));
}

TEST_F(StringHierarchyTest, BasicPathSingle) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_TRUE(string_hierarchy.in_hierarchy("root"));
}

TEST_F(StringHierarchyTest, ParentPath) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "child1"}));
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root"}));
}

TEST_F(StringHierarchyTest, InvalidAdjacentNode) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_FALSE(string_hierarchy.in_hierarchy({"root", "child1", "invalid_child"}));
}

TEST_F(StringHierarchyTest, InvalidRoot) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_FALSE(string_hierarchy.in_hierarchy({"badroot"}));
}

TEST_F(StringHierarchyTest, InvalidRootSingle) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_FALSE(string_hierarchy.in_hierarchy("badroot"));
}

TEST_F(StringHierarchyTest, NonExistantDepthNode) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    EXPECT_FALSE(string_hierarchy.in_hierarchy({"root", "child1", "child2", "child3"}));
}

TEST_F(StringHierarchyTest, AllExistDepthNode) {
    string_hierarchy.add_path({"root", "child1", "child2"}, true);
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "child1", "child2", "child3"}));
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "child1", "child2"}));
    EXPECT_FALSE(string_hierarchy.in_hierarchy({"root", "child1", "child2b"}));
}

TEST_F(StringHierarchyTest, DoubleNode) {
    string_hierarchy.add_path({"root", "child1", "child2"}, true);
    string_hierarchy.add_path({"root", "neighbor1", "child2"}, false);

    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "neighbor1", "child2"}));
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "child1", "child2"}));
    EXPECT_FALSE(string_hierarchy.in_hierarchy({"root", "child2"}));
}

TEST_F(StringHierarchyTest, OverwriteAllChildrenRoot) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    string_hierarchy.add_path({"root"}, true);
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "child1", "child2", "child3"}));
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"root", "somethingrandom"}));
}

TEST_F(StringHierarchyTest, AllowAll) {
    string_hierarchy.add_path({}, true);
    EXPECT_TRUE(string_hierarchy.in_hierarchy({"literally", "anything"}));
}

TEST_F(StringHierarchyTest, AllowAllSingle) {
    string_hierarchy.add_path({}, true);
    EXPECT_TRUE(string_hierarchy.in_hierarchy("everything"));
}

TEST_F(StringHierarchyTest, InvalidHierarchy) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    const StringHierarchy* sub_hierarchy = string_hierarchy.sub_hierarchy({"invalid"});
    EXPECT_EQ(sub_hierarchy, nullptr);
}

TEST_F(StringHierarchyTest, SubHierarchy) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    const StringHierarchy* sub_hierarchy = string_hierarchy.sub_hierarchy({"root", "child1"});
    EXPECT_NE(sub_hierarchy, nullptr);
    EXPECT_FALSE(sub_hierarchy->in_hierarchy("root"));
    EXPECT_FALSE(sub_hierarchy->in_hierarchy("child1"));
    EXPECT_TRUE(sub_hierarchy->in_hierarchy("child2"));
}

TEST_F(StringHierarchyTest, InvalidDeepHierarchy) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    const StringHierarchy* sub_hierarchy = string_hierarchy.sub_hierarchy({"invalid", "nonexistant"});
    EXPECT_EQ(sub_hierarchy, nullptr);
}

TEST_F(StringHierarchyTest, SubHierarchySingle) {
    string_hierarchy.add_path({"root", "child1", "child2"}, false);
    const StringHierarchy* sub_hierarchy = string_hierarchy.sub_hierarchy("root");
    EXPECT_NE(sub_hierarchy, nullptr);
    EXPECT_FALSE(sub_hierarchy->in_hierarchy("root"));
    EXPECT_TRUE(sub_hierarchy->in_hierarchy("child1"));
    EXPECT_TRUE(sub_hierarchy->in_hierarchy({"child1", "child2"}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}