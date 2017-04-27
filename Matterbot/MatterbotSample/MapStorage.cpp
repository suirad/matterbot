#include "MapStorage.h"
#include <mutex>
#include <future>
#include <iostream>
#include <memory>



namespace darius {
	using namespace std;
	using namespace lospi;
	MapStorage::MapStorage(const wstring &pass) :
		password{ string(pass.begin(),pass.end()) } {
		//sql = unique_ptr<SqlStorage>(new SqlStorage(pass));
	}

	MapStorage::~MapStorage() {
	}

	void MapStorage::die() {
		lock_guard<mutex> lock(mutlive);
		live = false;
	}

	int MapStorage::getCompleted() {
		lock_guard<mutex> lock(mutlv);
		return gennedlv;
	}

	void MapStorage::setMax(int max) {
		maxlv = max;
	}

	bool MapStorage::getLive() {
		lock_guard<mutex> lock(mutlive);
		return live;
	}


	void MapStorage::generate() {
		vector<future<
			vector<pair<uint128_t, vector<bitset<8>>>>*
			>> futs;
		shared_ptr<MapStorage> ptr(this);
		for (int i = 0; i < 9; i++) {
			futs.emplace_back(
				async(launch::async, &MapStorage::genlevel, this,
					i + 1, password, ptr, 0)
			);
		}

		int j = 0;
		for (auto &it : futs) {
			if (!getLive()) { it.wait(); continue; }
			auto gen = it.get();
			mutcache.lock();
			for (auto &itr : *gen) {
				if (!getLive()) { break; }
				cache.emplace(itr);
				j++;
				if (j > 100) {
					mutcache.unlock();
					mutcache.lock();
					j = 0;
				}
			}
			mutcache.unlock();
			if (gen) { delete gen; }
			if (!getLive()) { continue; }
			mutlv.lock();
			gennedlv++;
			cout << "Generated lv " << to_string(gennedlv) << endl;
			mutlv.unlock();
		}
		if (!getLive()) { return; }

		vector<
			vector<pair<uint128_t, vector<bitset<8>>>>*
		> finished; finished.reserve(4);

		for (int i = 9; i < maxlv + 10; i++) {
			futs.clear();
			futs.emplace_back(
				async(launch::async, &MapStorage::genlevel, this,
					i + 1, password, ptr, 'h')
			);
			futs.emplace_back(
				async(launch::async, &MapStorage::genlevel, this,
					i + 1, password, ptr, 's')
			);
			futs.emplace_back(
				async(launch::async, &MapStorage::genlevel, this,
					i + 1, password, ptr, 'o')
			);
			futs.emplace_back(
				async(launch::async, &MapStorage::genlevel, this,
					i + 1, password, ptr, 'j')
			);

			if (!finished.empty()) {
				j = 0;
				for (auto &it : finished) {
					mutcache.lock();
					for (auto &itr : *it) {
						if (!getLive()) { break; }
						cache.emplace(itr);
						j++;
						if (j > 100) {
							mutcache.unlock();
							mutcache.lock();
							j = 0;
						}
					}
					mutcache.unlock();
					if (it) { delete it; }
				}
				mutlv.lock();
				gennedlv++;
				cout << "Generated mlv " << to_string(gennedlv) << endl;
				mutlv.unlock();
				finished.clear();
			}

			for (auto &it : futs) {
				finished.emplace_back(it.get());
			}

			if (!getLive()) {
				break;
			}
		}

		if (getLive()) {
			j = 0;
			for (auto &it : finished) {
				mutcache.lock();
				for (auto &itr : *it) {
					if (!getLive()) { break; }
					cache.emplace(itr);
					j++;
					if (j > 100) {
						mutcache.unlock();
						mutcache.lock();
						j = 0;
					}
				}
				mutcache.unlock();
				if (it) { delete it; }
				if (!getLive()) { continue; }
			}
			mutlv.lock();
			gennedlv++;
			cout << "Generated final multilv" << to_string(gennedlv) << endl;
			mutlv.unlock();
		}


		while (getLive()) {
			this_thread::sleep_for(1s);
		}
	}

	bitset<8>& MapStorage::bitplace(bitset<8> &b, char c) {
		if (b.any()) {
			switch (c) {
			case 'h': b[4] = 1; break;
			case 's':b[5] = 1; break;
			case 'o':b[6] = 1; break;
			case 'j':b[7] = 1; break;
			}
		}
		else {
			switch (c) {
			case 'h': b[0] = 1; break;
			case 's':b[1] = 1; break;
			case 'o':b[2] = 1; break;
			case 'j':b[3] = 1; break;
			}
		}
		return b;
	}

	bool MapStorage::bitspace(bitset<8>&b) {
		return (b[0] || b[1] || b[2] || b[3]) == true &&
			(b[4] || b[5] || b[6] || b[7]) == false
			? true : false;
	}

	vector<pair<uint128_t, vector<bitset<8>>>>* MapStorage::genlevel(int lv, string &pass,
		shared_ptr<MapStorage> ptr, char start) {
		auto ret = new vector<pair<uint128_t, vector<bitset<8>>>>;
		bool skip_first;
		unique_ptr<Combinator> comb;
		vector<char> next; next.reserve(lv / 2 );
		if (start == 0) {
			comb = unique_ptr<Combinator>(new Combinator("hsoj", lv));
			skip_first = false;
		}
		else {
			comb = unique_ptr<Combinator>(new Combinator("hsoj", lv));
			do {
				next = comb->next();
			} while (next.back() != start);
			skip_first = true;
		}

		vector<bitset<8>> vec; vec.reserve(lv);
		string tmppass = ""; tmppass.reserve(lv + pass.length());
		while (comb->has_next()) {
			if (!ptr->getLive()) { return NULL; }
			if (skip_first) {
				skip_first = false;
			}
			else { next = comb->next(); }

			if (start > 0 && next.back() != start) { break; }

			for (auto &it : next) {
				if (vec.empty()) {
					vec.emplace_back(bitplace(bitset<8>(), it));
				}
				else {
					bitset<8> bits = vec.back();
					if (bitspace(bits)) {
						vec.pop_back();
						vec.push_back(bitplace(bits, it));
					}
					else {
						vec.emplace_back(bitplace(bitset<8>(), it));
					}
				}
				tmppass += it;
			}
			tmppass.append(pass);
			ret->emplace_back(
				minify_md5(compute_md5(tmppass.c_str(), (unsigned long)tmppass.size())),
				vec);
			tmppass.clear();
			vec.clear();
		}
		return ret;
	}


	wstring MapStorage::bitwstring(bitset<8> b) {
		wstring ret = L"";
		ret += b[0] ? L"h" : L"";
		ret += b[1] ? L"s" : L"";
		ret += b[2] ? L"o" : L"";
		ret += b[3] ? L"j" : L"";
		ret += b[4] ? L"h" : L"";
		ret += b[5] ? L"s" : L"";
		ret += b[6] ? L"o" : L"";
		ret += b[7] ? L"j" : L"";
		return ret;
	}

	vector<wstring>* MapStorage::getPassword(vector<string> &strs) {
		auto wpass = wstring(password.begin(), password.end());
		auto search = vector<uint128_t>();
		for (auto it : strs) { search.emplace_back(minify_md5(get_md5_from_str(it))); }
		auto found = vector<vector<bitset<8>>>();
		mutcache.lock();
		for (auto it : search) {
			auto find = cache.find(it);
			if (find != cache.end()) {
				found.emplace_back(find->second);
			}
		}
		mutcache.unlock();
		auto ret = new vector<wstring>;
		wstring str;
		for (auto it : found) {
			for (auto itc : it) {
				str += bitwstring(itc);
			}
			ret->emplace_back(str + wpass);
			str.clear();
		}
		return ret;
	}
}