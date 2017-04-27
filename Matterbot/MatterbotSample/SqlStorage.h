#pragma once
#include "sqlite_modern_cpp\sqlite_modern_cpp.h"

namespace darius {
	class SqlStorage {
	public:
		SqlStorage(const std::wstring &pass);
		~SqlStorage();
	private:
		std::unique_ptr<sqlite::database> db;

	};
}