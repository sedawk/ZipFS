#include <gtest/gtest.h>
#include "../zipfs/Util.hpp"

class UtilTest : public ::testing::Test {
};

TEST_F(UtilTest, test_string_to_string) {
	EXPECT_EQ("�׽�Ʈ", util::string::to_string(L"�׽�Ʈ"));
	EXPECT_EQ(u8"�׽�Ʈ", util::string::to_string(L"�׽�Ʈ", CP_UTF8));
}

TEST_F(UtilTest, test_string_to_wstring) {
	EXPECT_EQ(L"�׽�Ʈ", util::string::to_wstring("�׽�Ʈ"));
	EXPECT_EQ(L"�׽�Ʈ", util::string::to_wstring(u8"�׽�Ʈ", CP_UTF8));
}

TEST_F(UtilTest, test_string_ltrim) {
	EXPECT_EQ("�׽�Ʈ", util::string::ltrim("\\�׽�Ʈ", '\\'));
	EXPECT_EQ("�׽�Ʈ", util::string::ltrim("�׽�Ʈ", '\\'));
	EXPECT_EQ(L"�׽�Ʈ", util::string::ltrim(L"\\�׽�Ʈ", L'\\'));
	EXPECT_EQ(L"�׽�Ʈ", util::string::ltrim(L"�׽�Ʈ", L'\\'));
}

TEST_F(UtilTest, test_string_rtrim) {
	EXPECT_EQ("�׽�Ʈ", util::string::rtrim("�׽�Ʈ\\", '\\'));
	EXPECT_EQ("�׽�Ʈ", util::string::rtrim("�׽�Ʈ", '\\'));
	EXPECT_EQ(L"�׽�Ʈ", util::string::rtrim(L"�׽�Ʈ\\", L'\\'));
	EXPECT_EQ(L"�׽�Ʈ", util::string::rtrim(L"�׽�Ʈ", L'\\'));
}

TEST_F(UtilTest, test_string_trim) {
	EXPECT_EQ("�׽�Ʈ", util::string::trim("\\�׽�Ʈ\\", '\\'));
	EXPECT_EQ("�׽�Ʈ", util::string::trim("�׽�Ʈ", '\\'));
	EXPECT_EQ(L"�׽�Ʈ", util::string::trim(L"\\�׽�Ʈ\\", L'\\'));
	EXPECT_EQ(L"�׽�Ʈ", util::string::trim(L"�׽�Ʈ", L'\\'));
}

TEST_F(UtilTest, test_string_replace) {
	EXPECT_EQ("path/to/file", util::string::replace("path\\to\\file", '\\', '/'));
	EXPECT_EQ("path/to/file", util::string::replace("path/to/file", '\\', '/'));
	EXPECT_EQ(L"path/to/file", util::string::replace(L"path\\to\\file", L'\\', L'/'));
	EXPECT_EQ(L"path/to/file", util::string::replace(L"path/to/file", L'\\', L'/'));
}

TEST_F(UtilTest, test_string_endsWith) {
	EXPECT_FALSE(util::string::endsWith("path/to/file", "/"));
	EXPECT_TRUE(util::string::endsWith("path/to/", "/"));
	EXPECT_TRUE(util::string::endsWith("path/to/", ""));
}

TEST_F(UtilTest, test_filesystem_dirname) {
	EXPECT_EQ("path/to/", util::filesystem::dirname("path/to/file", '/'));
	EXPECT_EQ("", util::filesystem::dirname("file", '/'));
	EXPECT_EQ("", util::filesystem::dirname("", '/'));
}

TEST_F(UtilTest, test_filesystem_filename) {
	EXPECT_EQ("file", util::filesystem::filename("path/to/file", '/'));
	EXPECT_EQ("to/", util::filesystem::filename("path/to/", '/'));
	EXPECT_EQ("", util::filesystem::filename("", '/'));
}