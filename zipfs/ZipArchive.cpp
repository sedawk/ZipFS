#include "Util.hpp"
#include "ZipArchive.hpp"
#include "ZipDirectory.hpp"
#include "ZipRootDirectory.hpp"
#include "ZipFile.hpp"
#include "ZipPath.hpp"

namespace Zip {

	void Archive::open(const std::string& path, libzip::flags_t flags) {
		mArchive = std::make_shared<libzip::archive>(path, flags);

		for (auto it = mArchive->cbegin(); it != mArchive->cend(); it++) {
			if (it->comp_method != ZIP_CM_STORE) {
				throw std::invalid_argument("Compressed archive not supported");
			}
			if (it->encryption_method != ZIP_EM_NONE) {
				throw std::invalid_argument("Encrypted archive not supported");
			}
		}
	}

	void Archive::open(const std::wstring& path, libzip::flags_t flags) {
		open(util::string::to_string(path), flags);
	}

	static std::string normalize(const std::string& path) {
		return util::string::trim(util::string::replace(path, '\\', '/'), '/');
	}

	std::shared_ptr<Zip::Entry> Archive::get(const std::string& path) {
		auto pathToFile = normalize(path);
		auto pathToDirectory = pathToFile + "/";

		if (pathToFile == "") {
			return std::make_shared<Zip::RootDirectory>(mArchive);
		}
		if (mArchive->exists(pathToFile, ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437)) {
			return std::make_shared<Zip::File>(mArchive, pathToFile);
		}
		if (mArchive->exists(pathToDirectory, ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437)) {
			return std::make_shared<Zip::Directory>(mArchive, pathToDirectory);
		}
		return std::make_shared<Zip::Path>(mArchive, pathToFile);
	}

	std::shared_ptr<Zip::Entry> Archive::get(const std::wstring& path) {
		return get(util::string::to_string(path));
	}
}