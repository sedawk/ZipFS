#include <gtest/gtest.h>
#include "../zipfs/ZipArchive.hpp"

class ZipFileTest : public ::testing::Test {
protected:
	constexpr static const wchar_t* MULTIBYTE_STORED_NOENCRYPTED_ZIP = L"MB_STOR_NOENC.zip";
	constexpr static const wchar_t* FILE_PATH = L"\\폴더1\\파일2.txt";

	void SetUp() override {
		archive.open(MULTIBYTE_STORED_NOENCRYPTED_ZIP);
	}

	Zip::Archive archive;
};

TEST_F(ZipFileTest, testReadFile) {
	auto file = archive.get(FILE_PATH);
	char buffer[1024] = {};
	DWORD size = 0;
	file->readFile(buffer, sizeof(buffer), &size, 0, nullptr);
	EXPECT_EQ(15, size);
	EXPECT_STREQ("폴더1/파일2.txt", buffer);
}
