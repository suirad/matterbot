#pragma once
#include <map>
#include <bitset>
#include <mutex>
#include "MD5Utilities.h"
#include "SqlStorage.h"

namespace darius {
	struct PackedChar;
	class MapStorage {
	public: 
		MapStorage(const std::wstring &pass);
		~MapStorage();
		void die();
		int getCompleted();
		void setMax(int max);
		std::vector<std::wstring>* getPassword(std::vector<std::string> &strs);
		void generate();
		bool getLive();
	private:
		std::mutex mutcache, mutlv, mutlive, mutsql;
		std::map<lospi::uint128_t, std::vector<std::bitset<8>>>
			cache;
		int maxlv = 1;
		int gennedlv=0;
		const std::string password;
		bool live = true;
		//std::unique_ptr<SqlStorage> sql;

		bool bitspace(std::bitset<8>&b);
		std::bitset<8>& bitplace(std::bitset<8> &b, char c);
		std::wstring bitwstring(std::bitset<8> b);
		std::vector<std::pair<lospi::uint128_t, std::vector<std::bitset<8>>>>* 
			genlevel(int lv, std::string &pass, std::shared_ptr<MapStorage> ptr,
				char start = 0);
	};

}