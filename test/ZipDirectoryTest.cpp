#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "../zipfs/ZipArchive.hpp"
#include "../zipfs/ZipEntry.hpp"

class ZipDirectoryTest : public ::testing::Test {
protected:
	constexpr static const wchar_t* ZIP_FILE = L"MB_STOR_NOENC.zip";
	constexpr static const wchar_t* DIR_PATH = L"\\Æú´õ1";

	void SetUp() override {
		archive.open(ZIP_FILE);
		file = archive.get(DIR_PATH);
	}

	Zip::Archive archive;
	std::shared_ptr<Zip::Entry> file;
	DOKAN_FILE_INFO info = {};
};

static std::vector<std::wstring> foundFiles;

static int WINAPI fakeFillFindData(PWIN32_FIND_DATAW data, PDOKAN_FILE_INFO dokanFileInfo) {
	foundFiles.push_back(data->cFileName);
	return 0;
}

TEST_F(ZipDirectoryTest, testDirectoryFindFiles) {
	foundFiles.clear();
	EXPECT_EQ(STATUS_SUCCESS, file->findFiles(fakeFillFindData, nullptr));
	EXPECT_EQ(2, foundFiles.size());
}

TEST_F(ZipDirectoryTest, testGetFileInformation) {
	BY_HANDLE_FILE_INFORMATION stat = {};
	EXPECT_EQ(STATUS_SUCCESS, file->getFileInformation(&stat, nullptr));
	EXPECT_EQ(FILE_ATTRIBUTE_DIRECTORY, stat.dwFileAttributes);
	EXPECT_EQ(4068593408, stat.ftCreationTime.dwLowDateTime);
	EXPECT_EQ(4068593408, stat.ftLastAccessTime.dwLowDateTime);
	EXPECT_EQ(4068593408, stat.ftLastWriteTime.dwLowDateTime);
	EXPECT_EQ(0, stat.nFileSizeLow);
}

TEST_F(ZipDirectoryTest, testCreateFileOKWithDefaultOptions) {
	EXPECT_EQ(STATUS_SUCCESS, file->createFile(nullptr, 0, 0, 0, 0, 0, &info));
	EXPECT_TRUE(info.IsDirectory);
}

TEST_F(ZipDirectoryTest, testCreateFileFailsWithNonDirectoryOption) {
	EXPECT_EQ(STATUS_FILE_IS_A_DIRECTORY, file->createFile(nullptr, 0, 0, 0, 0, FILE_NON_DIRECTORY_FILE, &info));
}

TEST_F(ZipDirectoryTest, testCreateFileFailsWithCertainCreateOptions) {
	EXPECT_EQ(STATUS_OBJECT_NAME_COLLISION, file->createFile(nullptr, 0, 0, 0, CREATE_ALWAYS, 0, &info));
	EXPECT_EQ(STATUS_OBJECT_NAME_COLLISION, file->createFile(nullptr, 0, 0, 0, OPEN_ALWAYS, 0, &info));
}
