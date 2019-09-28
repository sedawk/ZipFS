#pragma once

#include "ZipDirectory.hpp"

namespace Zip {

	class RootDirectory : public Directory {
	public:
		explicit RootDirectory(std::shared_ptr<libzip::archive> archive);

		NTSTATUS DOKAN_CALLBACK getFileInformation(LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo) override;
	};
}