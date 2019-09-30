#include <gtest/gtest.h>
#include <memory>
#include "../zipfs/ZipArchive.hpp"
#include "../zipfs/ZipEntry.hpp"

class ZipPathTest : public ::testing::Test {
protected:
	constexpr static const wchar_t* ZIP_FILE = L"MB_STOR_NOENC.zip";
	constexpr static const wchar_t* UNKNOWN_PATH = L"\\Ćú´ő1\\unknown.file";

	void SetUp() override {
		archive.open(ZIP_FILE);
		file = archive.get(UNKNOWN_PATH);
	}

	Zip::Archive archive;
	std::shared_ptr<Zip::Entry> file;
	DOKAN_FILE_INFO info = {};
};

TEST_F(ZipPathTest, testCreateFile) {
	EXPECT_EQ(STATUS_OBJECT_NAME_NOT_FOUND, file->createFile(nullptr, 0, 0, 0, 0, 0, &info));
	EXPECT_EQ(STATUS_SUCCESS, file->createFile(nullptr, 0, 0, 0, CREATE_ALWAYS, 0, &info));
	EXPECT_EQ(STATUS_SUCCESS, file->createFile(nullptr, 0, 0, 0, OPEN_ALWAYS, 0, &info));
}
