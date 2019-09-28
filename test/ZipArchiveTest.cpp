#include <gtest/gtest.h>
#include <stdexcept>
#include "../zipfs/ZipArchive.hpp"

class ZipArchiveTest : public ::testing::Test {
protected:
	constexpr static const wchar_t* UNKNOWN_ZIP = L"unknown.zip";
	constexpr static const wchar_t* INVALID_ZIP = L"test.exe";
	constexpr static const wchar_t* MULTIBYTE_COMPRESSED_ENCRYPTED_ZIP = L"MB_COMP_ENC.zip";
	constexpr static const wchar_t* MULTIBYTE_COMPRESSED_NOENCRYPTED_ZIP = L"MB_COMP_NOENC.zip";
	constexpr static const wchar_t* MULTIBYTE_STORED_ENCRYPTED_ZIP = L"MB_STOR_ENC.zip";
	constexpr static const wchar_t* MULTIBYTE_STORED_NOENCRYPTED_ZIP = L"MB_STOR_NOENC.zip";

	constexpr static const wchar_t* UNKNOWN_PATH = L"\\path\\to\\unknown.file";
	constexpr static const wchar_t* ROOT_PATH = L"\\";
	constexpr static const wchar_t* DIR_PATH = L"\\폴더1";
	constexpr static const wchar_t* FILE_PATH = L"\\폴더1\\파일2.txt";
	constexpr static const wchar_t* LARGE_FILE_PATH = L"\\1MB.txt";

	void SetUp() override {
		archive.open(MULTIBYTE_STORED_NOENCRYPTED_ZIP);
	}

	void checkFileStatus(const std::wstring& from, 
		const char* path, std::uint64_t size, bool exist, bool isDirectory, bool isFile) {
		auto file = archive.get(from);
		EXPECT_EQ(path, file->getPath());
		EXPECT_EQ(size, file->getSize());
		EXPECT_EQ(exist, file->exist());
		EXPECT_EQ(isDirectory, file->isDirectory());
		EXPECT_EQ(isFile, file->isFile());
	}

	Zip::Archive archive;
};

TEST_F(ZipArchiveTest, testOpenFails) {
	EXPECT_THROW(archive.open(INVALID_ZIP), std::invalid_argument);
	EXPECT_THROW(archive.open(UNKNOWN_ZIP), std::invalid_argument);
	EXPECT_THROW(archive.open(MULTIBYTE_COMPRESSED_ENCRYPTED_ZIP), std::invalid_argument);
	EXPECT_THROW(archive.open(MULTIBYTE_COMPRESSED_NOENCRYPTED_ZIP), std::invalid_argument);
	EXPECT_THROW(archive.open(MULTIBYTE_STORED_ENCRYPTED_ZIP), std::invalid_argument);
}

TEST_F(ZipArchiveTest, testOpenPasses) {
	EXPECT_NO_THROW(archive.open(UNKNOWN_ZIP, ZIP_CREATE));
	EXPECT_NO_THROW(archive.open(MULTIBYTE_STORED_NOENCRYPTED_ZIP));
}

TEST_F(ZipArchiveTest, testGetUnknownPath) {
	checkFileStatus(UNKNOWN_PATH, "path/to/unknown.file", 0, false, false, false);
}

TEST_F(ZipArchiveTest, testGetRootPath) {
	checkFileStatus(ROOT_PATH, "", 0, true, true, false);
}

TEST_F(ZipArchiveTest, testGetDirPath) {
	checkFileStatus(DIR_PATH, "폴더1/", 0, true, true, false);
}

TEST_F(ZipArchiveTest, testGetFilePath) {
	checkFileStatus(FILE_PATH, "폴더1/파일2.txt", 15, true, false, true);
}

TEST_F(ZipArchiveTest, testGetLargeFilePath) {
	checkFileStatus(LARGE_FILE_PATH, "1MB.txt", 1024 * 1024, true, false, true);
}