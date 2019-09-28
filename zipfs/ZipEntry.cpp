#include "ZipEntry.hpp"

namespace Zip {

	Entry::Entry(std::shared_ptr<libzip::archive> archive, const std::string& path) :
		mArchive(archive), mPath(path) {
	}

	std::string Entry::getPath() const {
		return mPath;
	}

	std::uint64_t Entry::getSize() const {
		try {
			return mArchive->stat(getPath(), ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437).size;
		}
		catch (...) {
			return 0;
		}
	}

	std::time_t Entry::getLastModifiedTime() const {
		try {
			return mArchive->stat(getPath(), ZIP_FL_ENC_RAW | ZIP_FL_ENC_CP437).mtime;
		}
		catch (...) {
			return time(nullptr);
		}
	}

	std::shared_ptr<libzip::archive> Entry::getArchive() const {
		return mArchive;
	}
}