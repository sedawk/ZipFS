#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "../zipfs/ZipArchive.hpp"
#include "../zipfs/ZipEntry.hpp"

class ZipRootDirectoryTest : public ::testing::Test {
protected:
	constexpr static const wchar_t* ZIP_FILE = L"MB_STOR_NOENC.zip";
	constexpr static const wchar_t* ROOT_PATH = L"\\";

	void SetUp() override {
		archive.open(ZIP_FILE);
		file = archive.get(ROOT_PATH);
	}

	Zip::Archive archive;
	std::shared_ptr<Zip::Entry> file;
};

static std::vector<std::wstring> foundFiles;

static int WINAPI fakeFillFindData(PWIN32_FIND_DATAW data, PDOKAN_FILE_INFO dokanFileInfo) {
	foundFiles.push_back(data->cFileName);
	return 0;
}

TEST_F(ZipRootDirectoryTest, testFindFiles) {
	foundFiles.clear();
	EXPECT_EQ(STATUS_SUCCESS, file->findFiles(fakeFillFindData, nullptr));
	EXPECT_EQ(3, foundFiles.size());
}

TEST_F(ZipRootDirectoryTest, testGetFileInformation) {
	BY_HANDLE_FILE_INFORMATION stat = {};
	EXPECT_EQ(STATUS_SUCCESS, file->getFileInformation(&stat, nullptr));
	EXPECT_EQ(FILE_ATTRIBUTE_DIRECTORY, stat.dwFileAttributes);
	EXPECT_EQ(0, stat.ftCreationTime.dwLowDateTime);
	EXPECT_EQ(0, stat.ftLastAccessTime.dwLowDateTime);
	EXPECT_EQ(0, stat.ftLastWriteTime.dwLowDateTime);
	EXPECT_EQ(0, stat.nFileSizeLow);
}