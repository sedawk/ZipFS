#pragma once

#pragma warning(push, 0)
#include <dokan/dokan.h>
#pragma warning(pop)
#include <ctime>
#include <memory>
#include <string>
#include "zip.hpp"

namespace Zip {

	class Entry {
	public:
		Entry(std::shared_ptr<libzip::archive> archive, const std::string& path);

		std::string getPath() const;
		std::uint64_t getSize() const;
		std::time_t getLastModifiedTime() const;

		virtual bool exist() const = 0;
		virtual bool isDirectory() const = 0;
		virtual bool isFile() const = 0;

		virtual NTSTATUS DOKAN_CALLBACK createFile(PDOKAN_IO_SECURITY_CONTEXT SecurityContext, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual void DOKAN_CALLBACK closeFile(PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual void DOKAN_CALLBACK cleanup(PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK readFile(LPVOID Buffer, DWORD BufferLength, LPDWORD ReadLength, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK writeFile(LPCVOID Buffer, DWORD NumberOfBytesToWrite, LPDWORD NumberOfBytesWritten, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK flushFileBuffers(PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK getFileInformation(LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK findFiles(PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK deleteFile(PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK deleteDirectory(PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK moveFile(LPCWSTR NewFileName, BOOL ReplaceIfExisting, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK lockFile(LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK setEndOfFile(LONGLONG ByteOffset, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK setAllocationSize(LONGLONG AllocSize, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK setFileAttributes(DWORD FileAttributes, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK setFileTime(CONST FILETIME* CreationTime, CONST FILETIME* LastAccessTime, CONST FILETIME* LastWriteTime, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK unlockFile(LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK getFileSecurity(PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG BufferLength, PULONG LengthNeeded, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK setFileSecurity(PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG SecurityDescriptorLength, PDOKAN_FILE_INFO DokanFileInfo) = 0;
		virtual NTSTATUS DOKAN_CALLBACK findStreams(PFillFindStreamData FillFindStreamData, PDOKAN_FILE_INFO DokanFileInfo) = 0;

	protected:
		std::shared_ptr<libzip::archive> getArchive() const;

	private:
		std::shared_ptr<libzip::archive> mArchive;
		std::string mPath;
	};
}