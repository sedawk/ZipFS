#pragma once

#include <dokan/dokan.h>
#include <dokan/fileinfo.h>
#include <memory>
#include <string>
#include <vector>

namespace Dokan {

	class Interface {
	public:
		void start(int argc, wchar_t* argv[]);
		void start(const std::vector<std::wstring>& args);

	protected:
		virtual std::shared_ptr<DOKAN_OPTIONS> parse(const std::vector<std::wstring>& args) = 0;
		virtual std::shared_ptr<DOKAN_OPERATIONS> createOperations() = 0;
	};
}