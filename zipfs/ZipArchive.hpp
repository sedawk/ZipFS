#pragma once

#include <memory>
#include <string>
#include "zip.hpp"
#include "Util.hpp"
#include "ZipEntry.hpp"

namespace Zip {

	class Archive {
	public:
		void open(const std::string& path, libzip::flags_t flags = 0);
		void open(const std::wstring& path, libzip::flags_t flags = 0);

		std::shared_ptr<Zip::Entry> get(const std::string& path);
		std::shared_ptr<Zip::Entry> get(const std::wstring& path);

	private:
		std::shared_ptr<libzip::archive> mArchive;
	};
}