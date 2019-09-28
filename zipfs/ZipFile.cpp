#include "Util.hpp"
#include "ZipFile.hpp"
#include <iostream>

namespace Zip {

	File::File(std::shared_ptr<libzip::archive> archive, const std::string& path) :
		Entry(archive, path) {
	}

	bool File::exist() const {
		return true;
	}

	bool File::isDirectory() const {
		return false;
	}

	bool File::isFile() const {
		return true;
	}

	NTSTATUS DOKAN_CALLBACK File::createFile(PDOKAN_IO_SECURITY_CONTEXT SecurityContext, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PDOKAN_FILE_INFO DokanFileInfo) {
		if (DokanFileInfo->IsDirectory) {
			return STATUS_NOT_A_DIRECTORY;
		}
		if (CreateOptions & FILE_DIRECTORY_FILE) {
			return STATUS_NOT_A_DIRECTORY;
		}
		if (CreateDisposition == CREATE_ALWAYS || CreateDisposition == OPEN_ALWAYS) {
			return STATUS_OBJECT_NAME_COLLISION;
		}
		return STATUS_SUCCESS;
	}

	void DOKAN_CALLBACK File::closeFile(PDOKAN_FILE_INFO DokanFileInfo) {
		return;
	}

	void DOKAN_CALLBACK File::cleanup(PDOKAN_FILE_INFO DokanFileInfo) {
		if (DokanFileInfo->IsDirectory) {
			return;
		}
		if (!DokanFileInfo->DeleteOnClose) {
			return;
		}
		auto index = getArchive()->find(getPath(), ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437);
		getArchive()->remove(index);
	}

	NTSTATUS DOKAN_CALLBACK File::readFile(LPVOID Buffer, DWORD BufferLength, LPDWORD ReadLength, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) {
		auto file = getArchive()->open(getPath(), ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437);
		file.seek(Offset);
		*ReadLength = file.read(Buffer, BufferLength);
		return STATUS_SUCCESS;
	}

	NTSTATUS DOKAN_CALLBACK File::writeFile(LPCVOID Buffer, DWORD NumberOfBytesToWrite, LPDWORD NumberOfBytesWritten, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::flushFileBuffers(PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::getFileInformation(LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo) {
		auto stat = getArchive()->stat(getPath(), ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437);
		HandleFileInformation->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
		util::time::TimeToFileTime(stat.mtime, &HandleFileInformation->ftCreationTime);
		util::time::TimeToFileTime(stat.mtime, &HandleFileInformation->ftLastAccessTime);
		util::time::TimeToFileTime(stat.mtime, &HandleFileInformation->ftLastWriteTime);
		HandleFileInformation->nFileSizeHigh = reinterpret_cast<PLARGE_INTEGER>(&stat.size)->HighPart;
		HandleFileInformation->nFileSizeLow = reinterpret_cast<PLARGE_INTEGER>(&stat.size)->LowPart;
		return STATUS_SUCCESS;
	}

	NTSTATUS DOKAN_CALLBACK File::findFiles(PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_A_DIRECTORY;
	}

	NTSTATUS DOKAN_CALLBACK File::deleteFile(PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::deleteDirectory(PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::moveFile(LPCWSTR NewFileName, BOOL ReplaceIfExisting, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::setEndOfFile(LONGLONG ByteOffset, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::setAllocationSize(LONGLONG AllocSize, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::setFileAttributes(DWORD FileAttributes, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::setFileTime(const FILETIME* CreationTime, const FILETIME* LastAccessTime, const FILETIME* LastWriteTime, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::getFileSecurity(PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG BufferLength, PULONG LengthNeeded, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}

	NTSTATUS DOKAN_CALLBACK File::setFileSecurity(PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG SecurityDescriptorLength, PDOKAN_FILE_INFO DokanFileInfo) {
		return STATUS_NOT_IMPLEMENTED;
	}
}