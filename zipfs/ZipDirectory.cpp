#include "Util.hpp"
#include "ZipDirectory.hpp"

namespace Zip {
	Directory::Directory(std::shared_ptr<libzip::archive> archive, const std::string& path) :
		Entry(archive, path) {
	}

	bool Directory::exist() const {
		return true;
	}

	bool Directory::isDirectory() const {
		return true;
	}

	bool Directory::isFile() const {
		return false;
	}

	NTSTATUS DOKAN_CALLBACK Directory::createFile(PDOKAN_IO_SECURITY_CONTEXT SecurityContext, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_SUCCESS;
	}

	void DOKAN_CALLBACK Directory::closeFile(PDOKAN_FILE_INFO DokanFileInfo) {
		return;
	}

	void DOKAN_CALLBACK Directory::cleanup(PDOKAN_FILE_INFO DokanFileInfo) {
		return;
	}

	NTSTATUS DOKAN_CALLBACK Directory::readFile(LPVOID Buffer, DWORD BufferLength, LPDWORD ReadLength, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_FILE_IS_A_DIRECTORY;
	}

	NTSTATUS DOKAN_CALLBACK Directory::writeFile(LPCVOID Buffer, DWORD NumberOfBytesToWrite, LPDWORD NumberOfBytesWritten, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::flushFileBuffers(PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::getFileInformation(LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo) {
		libzip::stat stat = {};
		try {
			stat = getArchive()->stat(getPath(), ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437);
		}
		catch (...) {
		}
		HandleFileInformation->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		util::time::TimeToFileTime(stat.mtime, &HandleFileInformation->ftCreationTime);
		util::time::TimeToFileTime(stat.mtime, &HandleFileInformation->ftLastAccessTime);
		util::time::TimeToFileTime(stat.mtime, &HandleFileInformation->ftLastWriteTime);
		HandleFileInformation->nFileSizeHigh = reinterpret_cast<PLARGE_INTEGER>(&stat.size)->HighPart;
		HandleFileInformation->nFileSizeLow = reinterpret_cast<PLARGE_INTEGER>(&stat.size)->LowPart;
		return STATUS_SUCCESS;
	}

	static std::wstring wfilename(const std::string& path) {
		return util::string::to_wstring(util::filesystem::filename(util::string::trim(path, '/'), '/'));
	}

	NTSTATUS DOKAN_CALLBACK Directory::findFiles(PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo) {
		for (auto i = 0; i < getArchive()->num_entries(); i++) {
			auto stat = getArchive()->stat(i, ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437);
			if (getPath() != util::filesystem::dirname(stat.name, '/')) {
				continue;
			}
			WIN32_FIND_DATAW data = {};
			data.dwFileAttributes = (util::string::endsWith(stat.name, "/")) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL;
			wcscpy_s(data.cFileName, wfilename(stat.name).c_str());
			util::time::TimeToFileTime(stat.mtime, &data.ftCreationTime);
			util::time::TimeToFileTime(stat.mtime, &data.ftLastAccessTime);
			util::time::TimeToFileTime(stat.mtime, &data.ftLastWriteTime);
			data.nFileSizeHigh = reinterpret_cast<PLARGE_INTEGER>(&stat.size)->HighPart;
			data.nFileSizeLow = reinterpret_cast<PLARGE_INTEGER>(&stat.size)->LowPart;
			FillFindData(&data, DokanFileInfo);
		}
		return STATUS_SUCCESS;
	}

	NTSTATUS DOKAN_CALLBACK Directory::deleteFile(PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::deleteDirectory(PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::moveFile(LPCWSTR NewFileName, BOOL ReplaceIfExisting, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::lockFile(LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::setEndOfFile(LONGLONG ByteOffset, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::setAllocationSize(LONGLONG AllocSize, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::setFileAttributes(DWORD FileAttributes, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::setFileTime(const FILETIME* CreationTime, const FILETIME* LastAccessTime, const FILETIME* LastWriteTime, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::unlockFile(LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::getFileSecurity(PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG BufferLength, PULONG LengthNeeded, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::setFileSecurity(PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG SecurityDescriptorLength, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK Directory::findStreams(PFillFindStreamData FillFindStreamData, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}
}
