#include <iostream>
#include "SqlStorage.h"
#include "sqlite_modern_cpp\sqlite_modern_cpp.h"

namespace darius {
	using namespace std;
	using namespace sqlite;
	SqlStorage::SqlStorage(const wstring &pass) {
		//const string dir = "C:\\Users\\DOTlaptop\\Desktop\\sqls";
		//database db(dir + string(pass.begin(), pass.end()) + ".db");
		db = unique_ptr<database>(new database(":memory:"));
		*db << "create table if not exists hashes ("
			"md5 blob primarykey not null,"
			"salt text"
			");";
	}

	SqlStorage::~SqlStorage() {
		
	}

}