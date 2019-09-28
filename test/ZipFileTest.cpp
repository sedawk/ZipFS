#include <gtest/gtest.h>
#include <memory>
#include "../zipfs/ZipArchive.hpp"
#include "../zipfs/ZipEntry.hpp"

class ZipFileTest : public ::testing::Test {
protected:
	constexpr static const wchar_t* ZIP_FILE = L"MB_STOR_NOENC.zip";
	constexpr static const wchar_t* FILE_PATH = L"\\폴더1\\파일2.txt";

	void SetUp() override {
		archive.open(ZIP_FILE);
		file = archive.get(FILE_PATH);
	}

	Zip::Archive archive;
	std::shared_ptr<Zip::Entry> file;
	DOKAN_FILE_INFO info = {};
};

TEST_F(ZipFileTest, testGetFileInformation) {
	BY_HANDLE_FILE_INFORMATION stat = {};
	EXPECT_EQ(STATUS_SUCCESS, file->getFileInformation(&stat, nullptr));
	EXPECT_EQ(FILE_ATTRIBUTE_NORMAL, stat.dwFileAttributes);
	EXPECT_EQ(4128593408, stat.ftCreationTime.dwLowDateTime);
	EXPECT_EQ(4128593408, stat.ftLastAccessTime.dwLowDateTime);
	EXPECT_EQ(4128593408, stat.ftLastWriteTime.dwLowDateTime);
	EXPECT_EQ(15, stat.nFileSizeLow);
}

TEST_F(ZipFileTest, testReadFile) {
	char buffer[1024] = {};
	DWORD size = 0;
	EXPECT_EQ(STATUS_SUCCESS, file->readFile(buffer, sizeof(buffer), &size, 0, nullptr));
	EXPECT_EQ(15, size);
	EXPECT_STREQ("폴더1/파일2.txt", buffer);
}

TEST_F(ZipFileTest, testCreateFileFailsWithIsDirectoryTrue) {
	info.IsDirectory = TRUE;
	EXPECT_EQ(STATUS_NOT_A_DIRECTORY, file->createFile(nullptr, 0, 0, 0, 0, 0, &info));
}

TEST_F(ZipFileTest, testCreateFileOKWithIsDirectoryFalse) {
	info.IsDirectory = FALSE;
	EXPECT_EQ(STATUS_SUCCESS, file->createFile(nullptr, 0, 0, 0, 0, 0, &info));
}

TEST_F(ZipFileTest, testCreateFileFailsWithDirectoryOption) {
	EXPECT_EQ(STATUS_NOT_A_DIRECTORY, file->createFile(nullptr, 0, 0, 0, 0, FILE_DIRECTORY_FILE, &info));
}

TEST_F(ZipFileTest, testCreateFileFailsWhenCertainCreateOptions) {
	EXPECT_EQ(STATUS_OBJECT_NAME_COLLISION, file->createFile(nullptr, 0, 0, 0, CREATE_ALWAYS, 0, &info));
	EXPECT_EQ(STATUS_OBJECT_NAME_COLLISION, file->createFile(nullptr, 0, 0, 0, OPEN_ALWAYS, 0, &info));
}