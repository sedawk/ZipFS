#pragma once

#include "DokanInterface.hpp"

namespace Zip {

	class FS : public Dokan::Interface {
	protected:
		std::shared_ptr<DOKAN_OPTIONS> parse(const std::vector<std::wstring>& args) override;
		std::shared_ptr<DOKAN_OPERATIONS> createOperations() override;
	};
}